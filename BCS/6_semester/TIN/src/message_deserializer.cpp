/*
 * Project: TinyAd
 * Description: Message deserializing methods
 * Author: Damian Kolaska
 * Created: 30.04.2021
 */

#include "message_deserializer.hpp"

#include <cstring>
#include <map>

namespace TinyAd {

template <class T>
bool canDeserialize(StringConstIt begin, StringConstIt end) {
    return (std::size_t)std::distance(begin, end) >= sizeof(T);
}

bool canDeserialize(StringConstIt begin, StringConstIt end, std::size_t bytes) {
    return (std::size_t)std::distance(begin, end) >= bytes;
}

template <class T>
void deserialize(T* value, StringConstIt& begin, StringConstIt end) {
    if (!canDeserialize<T>(begin, end))
        throw std::runtime_error("Cannot correctly deserialize the message. Not enough bytes.");
    std::memcpy(value, &*begin, sizeof(T));
    std::advance(begin, sizeof(T));
}

void deserialize(std::back_insert_iterator<std::string> value, StringConstIt& begin,
                 StringConstIt end) {
    if (!canDeserialize<std::size_t>(begin, end))
        throw std::runtime_error("Cannot correctly deserialize the message. Not enough bytes.");
    std::size_t size;
    deserialize(&size, begin, end);
    if (!canDeserialize(begin, end, size))
        throw std::runtime_error("Cannot correctly deserialize the message. Not enough bytes.");
    std::copy(begin, std::next(begin, size), value);
    std::advance(begin, size);
}

template <MsgType T>
std::unique_ptr<Message> deserialize(StringConstIt& begin, StringConstIt end) {
    return deserialize<T>(begin, end);
}

template <>
std::unique_ptr<Message> deserialize<TYPE_MULTIMEDIA_HEADER>(StringConstIt& begin,
                                                             StringConstIt end) {
    auto msg = std::make_unique<MultimediaHeader>();
    std::advance(begin, sizeof(MsgType));
    deserialize(std::back_inserter(msg->id), begin, end);
    deserialize(&msg->filesize, begin, end);
    deserialize(&msg->num_chunks, begin, end);
    deserialize(std::back_inserter(msg->filename), begin, end);
    return msg;
}

template <>
std::unique_ptr<Message> deserialize<TYPE_MULTIMEDIA_DATA>(StringConstIt& begin,
                                                           StringConstIt end) {
    auto msg = std::make_unique<MultimediaData>();
    std::advance(begin, sizeof(MsgType));
    deserialize(std::back_inserter(msg->id), begin, end);
    deserialize(&msg->chunk_num, begin, end);
    deserialize(std::back_inserter(msg->data), begin, end);
    deserialize(&msg->crc, begin, end);
    return msg;
}

template <>
std::unique_ptr<Message> deserialize<TYPE_CONTROL_MESSAGE>(StringConstIt& begin,
                                                           StringConstIt end) {
    auto msg = std::make_unique<ControlMessage>();
    std::advance(begin, sizeof(MsgType));
    deserialize(std::back_inserter(msg->panel_id), begin, end);
    deserialize(&msg->control_msg_type, begin, end);
    deserialize(std::back_inserter(msg->file_id), begin, end);
    deserialize(&msg->chunk_num, begin, end);
    return msg;
}

template <>
std::unique_ptr<Message> deserialize<TYPE_SERVER_CONN_ACCEPT>(StringConstIt& begin,
                                                              StringConstIt end) {
    auto msg = std::make_unique<ServerConnectionAccept>();
    std::advance(begin, sizeof(MsgType));
    deserialize(std::back_inserter(msg->panel_id), begin, end);
    deserialize(std::back_inserter(msg->broadcast_address), begin, end);
    return msg;
}

template <>
std::unique_ptr<Message> deserialize<TYPE_PANEL_HELLO>(StringConstIt& begin, StringConstIt end) {
    auto msg = std::make_unique<PanelHello>();
    std::advance(begin, sizeof(MsgType));
    deserialize(std::back_inserter(msg->panel_id), begin, end);
    deserialize(std::back_inserter(msg->panel_config), begin, end);
    return msg;
}

// find first message and move iterator to the beginning of it
MsgType findFirstMsgType(StringConstIt& begin, StringConstIt end) {
    auto NotFound = [begin, end]() { return (std::size_t)(end - begin); };
    std::map<MsgType, std::size_t> msg_positions = {{TYPE_MULTIMEDIA_HEADER, NotFound()},
                                                    {TYPE_MULTIMEDIA_DATA, NotFound()},
                                                    {TYPE_CONTROL_MESSAGE, NotFound()},
                                                    {TYPE_SERVER_CONN_ACCEPT, NotFound()},
                                                    {TYPE_PANEL_HELLO, NotFound()}};
    for (auto& msg_pos : msg_positions) {  // for each type try to find it
        msg_pos.second = (std::size_t)(std::find(begin, end, (char)msg_pos.first) -
                                       begin);  // find - begin -> position
    }
    // find first message
    auto first_msg = std::min_element(msg_positions.begin(), msg_positions.end(),
                                      [NotFound](const auto& a, const auto& b) {
                                          return (a.second < b.second) && (a.second != NotFound());
                                      });
    if (first_msg == msg_positions.end() || first_msg->second == NotFound()) throw NoMessageFound();
    std::advance(begin,
                 first_msg->second);  // advance iterator to the beginning of the first message
    return first_msg->first;
}

std::unique_ptr<Message> deserialize(StringConstIt& begin, StringConstIt end) {
    auto msg_type = findFirstMsgType(begin, end);
    switch (msg_type) {
        case (TYPE_MULTIMEDIA_HEADER):
            return deserialize<TYPE_MULTIMEDIA_HEADER>(begin, end);
        case (TYPE_MULTIMEDIA_DATA):
            return deserialize<TYPE_MULTIMEDIA_DATA>(begin, end);
        case (TYPE_CONTROL_MESSAGE):
            return deserialize<TYPE_CONTROL_MESSAGE>(begin, end);
        case (TYPE_SERVER_CONN_ACCEPT):
            return deserialize<TYPE_SERVER_CONN_ACCEPT>(begin, end);
        case (TYPE_PANEL_HELLO):
            return deserialize<TYPE_PANEL_HELLO>(begin, end);
        default:
            return nullptr;
    }
}

}  // namespace TinyAd
