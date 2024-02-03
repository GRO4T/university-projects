/*
 * Project: TinyAd
 * Description: Message structures
 * Author: Damian Kolaska
 * Created: 27.04.2021
 */

#ifndef MESSAGE_HPP
#define MESSAGE_HPP

#include <algorithm>

#include "common.hpp"

namespace TinyAd {

using MsgType = uint8_t;
const MsgType TYPE_MULTIMEDIA_DATA = 0;
const MsgType TYPE_MULTIMEDIA_HEADER = 1;
const MsgType TYPE_CONTROL_MESSAGE = 2;
const MsgType TYPE_PANEL_HELLO = 3;
const MsgType TYPE_SERVER_CONN_ACCEPT = 4;

using ControlMsgType = uint8_t;
// to server
const ControlMsgType CONTROL_NAK = 0;
const ControlMsgType CONTROL_FILE_HEADER_REQUESTED = 1;
const ControlMsgType CONTROL_SLOW_DOWN_TRANSMISSION = 2;
// to client
const ControlMsgType CONTROL_FILE_NOT_FOUND = 3;

const uint32_t MULTIMEDIA_DATA_SIZE = 512;

struct Message {
    virtual MsgType GetType() = 0;
};

using IdType = std::string;

struct MultimediaHeader : public Message {
    MultimediaHeader() {}
    MultimediaHeader(IdType id, uint32_t filesize, uint32_t num_chunks, const std::string& filename)
        : id(id), filesize(filesize), num_chunks(num_chunks), filename(filename) {}

    MsgType GetType() { return TYPE_MULTIMEDIA_HEADER; }

    friend std::ostream& operator<<(std::ostream& os, const MultimediaHeader& mh) {
        os << fmt::format("id: {}, filesize: {}, num_chunks: {}, filename: {}", mh.id, mh.filesize,
                          mh.num_chunks, mh.filename);
        return os;
    }

    IdType id;
    uint32_t filesize;
    uint32_t num_chunks;
    std::string filename;
};

struct MultimediaData : public Message {
    MultimediaData() {}
    MultimediaData(IdType id, uint32_t chunk_num, const std::string& data)
        : id(id), chunk_num(chunk_num) {
        if (data.length() > MULTIMEDIA_DATA_SIZE) throw std::runtime_error("Data too large");
        std::copy(data.begin(), data.end(), std::back_inserter(this->data));
        crc = calculateCRC(data);
    }

    MsgType GetType() { return TYPE_MULTIMEDIA_DATA; }

    friend std::ostream& operator<<(std::ostream& os, const MultimediaData& md) {
        os << fmt::format("id: {}, chunk_num: {}, crc: {}", md.id, md.chunk_num, md.crc);
        return os;
    }

    IdType id;
    uint32_t chunk_num;
    std::string data;
    uint32_t crc;
};

struct ControlMessage : public Message {
    ControlMessage() {}
    ControlMessage(const std::string& panelId, ControlMsgType controlMsgType, IdType fileId = "")
        : panel_id(panelId), control_msg_type(controlMsgType), file_id(fileId) {}
    ControlMessage(const std::string& panelId, IdType fileId, int chunkNum)
        : panel_id(panelId), control_msg_type(CONTROL_NAK), file_id(fileId), chunk_num(chunkNum) {}

    MsgType GetType() { return TYPE_CONTROL_MESSAGE; }

    std::string panel_id;
    ControlMsgType control_msg_type = 0;
    IdType file_id = "";
    int chunk_num = 0;
};

struct ServerConnectionAccept : public Message {
    ServerConnectionAccept() {}
    ServerConnectionAccept(const std::string& panelId, const std::string& broadcastAddress)
        : panel_id(panelId), broadcast_address(broadcastAddress) {}

    MsgType GetType() { return TYPE_SERVER_CONN_ACCEPT; }

    std::string panel_id;
    std::string broadcast_address;
};

struct PanelHello : public Message {
    PanelHello() {}
    PanelHello(const std::string& panelId, const std::string& panelConfig)
        : panel_id(panelId), panel_config(panelConfig) {}

    MsgType GetType() { return TYPE_PANEL_HELLO; }

    std::string panel_id;
    std::string panel_config;
};

}  // namespace TinyAd

#endif