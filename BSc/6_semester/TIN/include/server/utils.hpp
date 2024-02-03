/*
 * Project: TinyAd
 * Description: Utility functions for server
 * Author: Rafal Kulus
 * Created: 04.06.2021
 */

#ifndef SERVER_UTILS_HPP
#define SERVER_UTILS_HPP

#include <unistd.h>

#include <chrono>
#include <filesystem>
#include <map>
#include <memory>
#include <string>

#include "config.hpp"
#include "file_serializer.hpp"
#include "message.hpp"
#include "message_serializer.hpp"
#include "panel_config.hpp"
#include "udp_connection.hpp"

using namespace TinyAd;

//
// UTILS
//

typedef struct {
    PanelConfig config;
    time_t last_hello;
    sockaddr_in6 addr;
} PanelInfo;

struct ServerContext {
    ulong last_slowdown;
    ulong last_acceleration;
    const uint32_t slowdown_rate = 10;
    const uint32_t acceleration_rate = 10;
    const ulong slowdown_interval = 100000;
    const ulong transmission_acceleration_interval = 1000000;
    const ulong max_delay = 5000;
    const ulong min_delay = 1;
    uint32_t delay = min_delay;
};

typedef std::map<std::string, std::pair<bool, std::shared_ptr<FileSerializer>>> FileSerializerMap;
typedef std::map<std::string, PanelInfo> PanelInfoMap;

ulong getTimeInMicroseconds() {
    using namespace std::chrono;
    return duration_cast<microseconds>(steady_clock::now().time_since_epoch()).count();
}

void sendFile(UdpConnection& conn, FileSerializerMap& fss, const std::string& file_path,
              ServerContext& serverContext, const std::optional<sockaddr_in6>& to = {}) {
    auto fs = std::make_shared<FileSerializer>(file_path);
    MultimediaHeader header = fs->getFileHeader();

    const auto restricted = to.has_value();
    fss.insert_or_assign(header.id,
                         std::pair<bool, std::shared_ptr<FileSerializer>>(restricted, fs));

    int sent = -1;
    if (to) {
        sent = conn.sendMsgTo(serialize(header), &to.value());
    } else {
        sent = conn.sendMsg(serialize(header));
    }

    DLOG_S(INFO) << fmt::format("Sent {} bytes of MultimediaHeader", sent);
    DLOG_S(INFO) << fmt::format("{} chunks awaiting", header.num_chunks);

    while (fs->getPartsLeft()) {
        // Otherwise clients will be dropping datagrams
        usleep(serverContext.delay);
        MultimediaData data = fs->getNextFileData();
        DLOG_S(INFO) << fmt::format("chunk: {}", data.chunk_num);

        if (to) {
            sent = conn.sendMsgTo(serialize(data), &to.value());
        } else {
            sent = conn.sendMsg(serialize(data));
        }

        DLOG_S(INFO) << fmt::format("Sent {} bytes of MultimediaData", sent);
    }
}

void sendPanicContent(UdpConnection& conn, FileSerializerMap& fss, ServerContext& context,
                      const std::string& file_path, const std::vector<PanelInfo>& recipients) {
    const auto now = std::time(0);

    std::filesystem::create_directory("tmp");
    std::ofstream temp_file("./tmp/panic_sched.sched");
    temp_file << "file=" << file_path << "\n";
    temp_file.close();

    for (const auto& panel : recipients) {
        const int delta_time = now - panel.last_hello;
        std::cout << fmt::format("Sending important content to panel that was active {}s ago.\n",
                                 delta_time);

        sendFile(conn, fss, file_path, context, panel.addr);
        sendFile(conn, fss, "./tmp/panic_sched.sched", context, panel.addr);
    }
}

void panicSendByPanelName(UdpConnection& conn, FileSerializerMap& fss, ServerContext& context,
                          const std::string& file_path, const PanelInfoMap& panel_info_map,
                          const std::string& panel_name) {
    std::vector<PanelInfo> recipients{};
    for (const auto& [_, panel_info] : panel_info_map) {
        if (panel_info.config.getName() == panel_name) {
            recipients.emplace_back(panel_info);
        }
    }

    sendPanicContent(conn, fss, context, file_path, recipients);
}

void panicSendByTags(UdpConnection& conn, FileSerializerMap& fss, ServerContext& context,
                     const std::string& file_path, const PanelInfoMap& panel_info_map,
                     const std::vector<std::string>& tags) {
    std::vector<PanelInfo> recipients{};
    for (const auto& [_, panel_info] : panel_info_map) {
        for (const auto& tag : tags) {
            if (panel_info.config.hasTag(tag)) {
                recipients.emplace_back(panel_info);
                break;
            }
        }
    }

    sendPanicContent(conn, fss, context, file_path, recipients);
}

std::string tail(const std::string filepath, int n) {
    std::ifstream file(filepath);
    if (!file.is_open()) throw new std::runtime_error(fmt::format("Cannot open file {}", filepath));

    file.seekg(0, std::ios::end);
    const std::size_t size = file.tellg();

    std::size_t pos = size;
    int count = 0;
    while (pos) {
        file.seekg(--pos);
        if (file.get() == '\n')
            if (count++ == n) break;
    }
    std::string lines(size - pos - 1, 'A');
    file.read((char*)lines.data(), size - pos - 1);

    return lines;
}

//
// COMPUTE
//

void handleNakAndFileHeaderRequested(ControlMessage* msg, UdpConnection& conn,
                                     FileSerializerMap& fss, const PanelInfoMap& panel_info_map) {
    auto opt_fs = getValueFromMap(fss, msg->file_id);
    if (opt_fs.has_value()) {
        auto [restricted, fs] = opt_fs.value();

        std::optional<sockaddr_in6> to{};
        if (restricted) {
            const auto panel_info = getValueFromMap(panel_info_map, msg->panel_id);
            if (!panel_info) {
                return;
            }

            to = panel_info.value().addr;
        }

        if (msg->control_msg_type == CONTROL_NAK) {
            auto res = fs->getFileData(msg->chunk_num);
            to ? conn.sendMsgTo(serialize(res), &to.value()) : conn.sendMsg(serialize(res));
        } else {
            auto res = fs->getFileHeader();
            to ? conn.sendMsgTo(serialize(res), &to.value()) : conn.sendMsg(serialize(res));
        }
        DLOG_S(INFO) << "Got NAK / FILE_HEADER_REQUESTED, sent MultimediaData or "
                        "MultimediaHeader";
    } else {
        ControlMessage res("", CONTROL_FILE_NOT_FOUND, msg->file_id);
        conn.sendMsg(serialize(res));
        DLOG_S(INFO) << "Got NAK / FILE_HEADER_REQUESTED, sent CONTROL_FILE_NOT_FOUND";
    }
}

void handlePanelHello(std::unique_ptr<Message> msg, UdpConnection& conn, PanelInfoMap& pi,
                      sockaddr_in6& from) {
    auto hello_msg = static_cast<PanelHello*>(msg.get());
    try {
        PanelConfig panel_config(hello_msg->panel_config);
        // Update panel info by last time it was active (so basically now)
        PanelInfo panel_info{panel_config, std::time(0), from};
        pi.insert_or_assign(hello_msg->panel_id, panel_info);
        ServerConnectionAccept res(hello_msg->panel_id, MC_IP);
        conn.sendMsgTo(serialize(res), &from);
        DLOG_S(INFO) << "Got HELLO";
    } catch (ConfigParserException& e) {
        LOG_S(ERROR) << fmt::format("Exception parsing panel config: {}", e.what());
        return;
    }
}

#endif