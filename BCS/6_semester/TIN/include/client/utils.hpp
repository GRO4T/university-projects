/*
 * Project: TinyAd
 * Description: Utility functions for client
 * Author: Rafal Kulus
 * Created: 04.06.2021
 */

#ifndef CLIENT_UTILS_HPP
#define CLIENT_UTILS_HPP

#include <map>
#include <memory>
#include <optional>
#include <set>
#include <string>

#include "file_deserializer.hpp"
#include "message.hpp"
#include "message_serializer.hpp"
#include "network_utils.hpp"
#include "schedule.hpp"
#include "udp_connection.hpp"

using namespace TinyAd;

typedef std::map<std::string, std::optional<std::shared_ptr<FileDeserializer>>> FileDeserializerMap;
typedef std::set<std::string> MissingHeadersSet;

//
// COMPUTE
//

void handleMultimediaHeader(std::unique_ptr<Message> msg, FileDeserializerMap& fds,
                            MissingHeadersSet& mh, std::string& new_sched_id) {
    auto mult_header = static_cast<MultimediaHeader*>(msg.get());
    DLOG_S(INFO) << "Received multimedia header [" << *mult_header << "]";

    if (fds.count(mult_header->id) != 0) {
        DLOG_S(INFO) << "Already exists!";
        return;
    }

    try {
        DLOG_S(INFO) << "Will be saved to ./recvd/" << mult_header->filename;
        auto fd = std::make_unique<FileDeserializer>(*mult_header, "./recvd/");
        fds.insert({mult_header->id, std::move(fd)});

        mh.erase(mult_header->id);

        std::string filename = mult_header->filename;
        if (filename.substr(filename.find_last_of(".") + 1) == "sched") {
            // Remove last schedule from deserializer, because it's no longer needed
            // Should also remove assets, but I don't have time for that
            fds.insert_or_assign(new_sched_id, std::nullopt);
            new_sched_id = mult_header->id;
        }
    } catch (std::exception& e) {
        LOG_S(ERROR) << fmt::format("Error handling multimedia header: {}", e.what());
    }
}

void handleMultimediaData(std::unique_ptr<Message> msg, FileDeserializerMap& fds,
                          MissingHeadersSet& mh, const std::string& new_sched_id,
                          std::optional<Schedule>& schedule) {
    auto mult_data = static_cast<MultimediaData*>(msg.get());
    DLOG_S(INFO) << "Received multimedia data chunk [" << *mult_data << "]";

    // if CRC does not match ignore this message. NAK will be sent later
    if (mult_data->crc != calculateCRC(mult_data->data)) {
        LOG_S(ERROR) << "CRC checksum does not match for " << *mult_data;
        return;
    }

    auto opt_fdo = getValueFromMap(fds, mult_data->id);
    if (opt_fdo.has_value()) {
        auto fdo = opt_fdo.value();
        if (fdo.has_value()) {
            auto fd = fdo.value();
            // prev_state is only to check if this MultimediaData completes the file
            // Otherwise 'Download successful' would fire every time client gets MultimediaData
            // for already completed file
            bool prev_state = fd->isCompleted();
            fd->addPart(*mult_data);

            if (!prev_state && fd->isCompleted()) {
                DLOG_S(INFO) << "Download successful!";

                if (mult_data->id == new_sched_id) {
                    DLOG_S(INFO) << "New schedule received!";
                    schedule = Schedule{fd->getFilePath()};
                    DLOG_S(INFO) << schedule.value().toString();
                }
            }
        }
    } else {
        mh.insert(mult_data->id);
        DLOG_S(INFO) << "got data for an unknown file";
    }
}

void handleControlFileNotFound(ControlMessage* msg, FileDeserializerMap& fds,
                               MissingHeadersSet& mh) {
    // Add empty optional to indicate, that client knows about the file,
    // but the file is impossible to get
    fds.insert_or_assign(msg->file_id, std::nullopt);
    mh.erase(msg->file_id);
}

void handleServerConnAccept(std::unique_ptr<Message> msg, UdpConnection& conn,
                            bool& joined_mc_group) {
    if (!joined_mc_group) {
        auto server_conn_accept = static_cast<ServerConnectionAccept*>(msg.get());
        conn.joinMulticastGroup(strToAddr(server_conn_accept->broadcast_address));
        joined_mc_group = true;
    }
}

//
// TICK
//

// Sends a single but random NAK for every file which has at least one part missing
void sendRandomNaks(UdpConnection& conn, FileDeserializerMap& fds, std::string& panel_id) {
    for (const auto& fdo : fds) {
        if (fdo.second.has_value()) {
            const auto& fd = fdo.second.value();
            if (fd->getRemainingPartsCount()) {
                int idx = UniformIntDistribution(0, fd->getRemainingPartsCount() - 1).get();
                int part = fd->getRemainingParts()[idx];
                ControlMessage msg(panel_id, fdo.first, part);
                conn.sendMsg(serialize(msg));
            }
        }
    }
}

void sendSingleFileHeaderRequested(UdpConnection& conn, MissingHeadersSet& mh,
                                   std::string& panel_id) {
    if (!mh.empty()) {
        ControlMessage msg(panel_id, CONTROL_FILE_HEADER_REQUESTED, *mh.cbegin());
        conn.sendMsg(serialize(msg));
    }
}

void printActiveFiles(FileDeserializerMap& fds) {
    DLOG_S(INFO) << "Active files:";
    for (auto fdp : fds) {
        if (fdp.second.has_value()) {
            auto fd = fdp.second.value();
            int received = fd->getAllPartsCount() - fd->getRemainingPartsCount();
            DLOG_S(INFO) << fmt::format("{} {} recved {} out of {} --- remaining {}", fdp.first,
                                        fd->getFilePath(), received, fd->getAllPartsCount(),
                                        fd->getRemainingPartsCount());
        }
    }
}

void printCurrentContent(const std::optional<Schedule>& schedule) {
    if (schedule) {
        const auto& current_entry = schedule.value().getValidEntry();
        if (current_entry) {
            LOG_S(INFO) << fmt::format("Displaying content from file {}...",
                                       current_entry.value().getFilename());
        } else {
            LOG_S(INFO) << "There is no valid content for this moment!";
        }
    } else {
        LOG_S(INFO) << "Schedule has not arrived yet!";
    }
}

#endif