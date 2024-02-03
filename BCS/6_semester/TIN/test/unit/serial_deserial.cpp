/*
 * Project: TinyAd
 * Description: Message serialization and deserialization tests
 * Author: Damian Kolaska
 * Created: 30.04.2021
 */

#include <gtest/gtest.h>

#include <cstring>
#include <type_traits>

#include "message.hpp"
#include "message_deserializer.hpp"
#include "message_serializer.hpp"
#include "uuid.h"

using namespace TinyAd;

TEST(SerializerDeserializer, OneMultimediaHeader) {
    MultimediaHeader msg("123", 123, 123, "hello.mkv");
    auto bytes = serialize(msg);
    auto c_it = bytes.cbegin();
    auto recv_msg = deserialize(c_it, bytes.cend());
    EXPECT_EQ(recv_msg->GetType(), msg.GetType());
    auto recv_mult_header = static_cast<MultimediaHeader*>(recv_msg.get());
    EXPECT_EQ(recv_mult_header->filesize, msg.filesize);
    EXPECT_EQ(recv_mult_header->filename, msg.filename);
}

TEST(SerializerDeserializer, OneMultimediaData) {
    MultimediaData msg("123", 1, "some_funky_ad_data");
    auto bytes = serialize(msg);
    auto c_it = bytes.cbegin();
    auto recv_msg = deserialize(c_it, bytes.cend());
    EXPECT_EQ(recv_msg->GetType(), msg.GetType());
    auto recv_mult_data = static_cast<MultimediaData*>(recv_msg.get());
    EXPECT_EQ(recv_mult_data->data, msg.data);
}

TEST(SerializerDeserializer, OneControlMessage) {
    ControlMessage msg("test_uuid", "123", 456);
    auto bytes = serialize(msg);
    auto c_it = bytes.cbegin();
    auto recv_msg = deserialize(c_it, bytes.cend());
    EXPECT_EQ(recv_msg->GetType(), msg.GetType());
    auto recv_ctl_msg = static_cast<ControlMessage*>(recv_msg.get());
    EXPECT_EQ(recv_ctl_msg->panel_id, msg.panel_id);
    EXPECT_EQ(recv_ctl_msg->file_id, msg.file_id);
    EXPECT_EQ(recv_ctl_msg->chunk_num, msg.chunk_num);
}

TEST(SerializerDeserializer, OneServConnAccept) {
    ServerConnectionAccept msg("test_uuid", "test_broadcast_address");
    auto bytes = serialize(msg);
    auto c_it = bytes.cbegin();
    auto recv_msg = deserialize(c_it, bytes.cend());
    EXPECT_EQ(recv_msg->GetType(), msg.GetType());
    auto recv_srv_conn_accept = static_cast<ServerConnectionAccept*>(recv_msg.get());
    EXPECT_EQ(recv_srv_conn_accept->panel_id, msg.panel_id);
    EXPECT_EQ(recv_srv_conn_accept->broadcast_address, msg.broadcast_address);
}

TEST(SerializerDeserializer, OnePanelHello) {
    PanelHello msg("test_uuid", "test_panel_config");
    auto bytes = serialize(msg);
    auto c_it = bytes.cbegin();
    auto recv_msg = deserialize(c_it, bytes.cend());
    EXPECT_EQ(recv_msg->GetType(), msg.GetType());
    auto recv_panel_hello = static_cast<PanelHello*>(recv_msg.get());
    EXPECT_EQ(recv_panel_hello->panel_id, msg.panel_id);
    EXPECT_EQ(recv_panel_hello->panel_config, msg.panel_config);
}

TEST(SerializerDeserializer, NoMessage) {
    std::string bytes = "";
    auto c_it = bytes.cbegin();
    EXPECT_THROW(auto deserial_msg = deserialize(c_it, bytes.cend());, NoMessageFound);
}

TEST(SerializerDeserializer, MultHeaderThenMultData) {
    MultimediaHeader msg1("123", 123, 123, "hello.mkv");
    MultimediaData msg2("123", 1, "some_funky_ad_data");
    auto bytes = serialize(msg1);
    bytes += serialize(msg2);
    auto c_it = bytes.cbegin();
    auto recv_msg1 = deserialize(c_it, bytes.cend());
    auto recv_msg2 = deserialize(c_it, bytes.cend());
    EXPECT_EQ(recv_msg1->GetType(), msg1.GetType());
    EXPECT_EQ(recv_msg2->GetType(), msg2.GetType());
}

TEST(SerializerDeserializer, CorruptedData) {
    MultimediaHeader msg1("123", 123, 123, "hello.mkv");
    MultimediaData msg2("123", 1, "some_funky_ad_data");
    std::string bytes = "some_corrupted_data";
    bytes += serialize(msg1);
    bytes += serialize(msg2);
    auto c_it = bytes.cbegin();
    auto recv_msg1 = deserialize(c_it, bytes.cend());
    auto recv_msg2 = deserialize(c_it, bytes.cend());
    EXPECT_EQ(recv_msg1->GetType(), msg1.GetType());
    EXPECT_EQ(recv_msg2->GetType(), msg2.GetType());
}