/*
 * Project: TinyAd
 * Description: Message serializing methods
 * Author: Damian Kolaska
 * Created: 30.04.2021
 */

#ifndef MESSAGE_SERIALIZER_HPP
#define MESSAGE_SERIALIZER_HPP

#include <algorithm>
#include <cstring>
#include <iterator>

#include "message.hpp"

namespace TinyAd {

template <class T>
void serialize(std::back_insert_iterator<std::string> buffer, T value) {
    std::copy_n((char*)&value, sizeof(T), buffer);
}

void serialize(std::back_insert_iterator<std::string> buffer, const std::string& value) {
    serialize(buffer, value.length());
    std::copy(value.begin(), value.end(), buffer);
}

std::string serialize(MultimediaHeader& msg) {
    std::string bytes;
    serialize(std::back_inserter(bytes), msg.GetType());
    serialize(std::back_inserter(bytes), msg.id);
    serialize(std::back_inserter(bytes), msg.filesize);
    serialize(std::back_inserter(bytes), msg.num_chunks);
    serialize(std::back_inserter(bytes), msg.filename);
    return bytes;
}

std::string serialize(MultimediaData& msg) {
    std::string bytes;
    serialize(std::back_inserter(bytes), msg.GetType());
    serialize(std::back_inserter(bytes), msg.id);
    serialize(std::back_inserter(bytes), msg.chunk_num);
    serialize(std::back_inserter(bytes), msg.data);
    serialize(std::back_inserter(bytes), msg.crc);
    return bytes;
}

std::string serialize(ControlMessage& msg) {
    std::string bytes;
    serialize(std::back_inserter(bytes), msg.GetType());
    serialize(std::back_inserter(bytes), msg.panel_id);
    serialize(std::back_inserter(bytes), msg.control_msg_type);
    serialize(std::back_inserter(bytes), msg.file_id);
    serialize(std::back_inserter(bytes), msg.chunk_num);
    return bytes;
}

std::string serialize(ServerConnectionAccept& msg) {
    std::string bytes;
    serialize(std::back_inserter(bytes), msg.GetType());
    serialize(std::back_inserter(bytes), msg.panel_id);
    serialize(std::back_inserter(bytes), msg.broadcast_address);
    return bytes;
}

std::string serialize(PanelHello& msg) {
    std::string bytes;
    serialize(std::back_inserter(bytes), msg.GetType());
    serialize(std::back_inserter(bytes), msg.panel_id);
    serialize(std::back_inserter(bytes), msg.panel_config);
    return bytes;
}

}  // namespace TinyAd

#endif