#include <gtest/gtest.h>

#include <cstring>
#include <type_traits>

#include "tuple.hpp"
#include "serializer.hpp"
#include "deserializer.hpp"

using namespace linda;

TEST(Tuple, StringAll) {
    String pattern("*");
    TupleElem elemTuple("przyklad");
    EXPECT_EQ(pattern.matches(elemTuple), true);
}

TEST(Tuple, StringEqual) {
    String pattern("==przyklad");
    TupleElem elemTupleEqual("przyklad");
    TupleElem elemTupleNotEqual("przyklad2");
    EXPECT_EQ(pattern.matches(elemTupleEqual), true);
    EXPECT_EQ(pattern.matches(elemTupleNotEqual), false);
}

TEST(Tuple, StringNotEqual) {
    String pattern("!=przyklad");
    TupleElem elemTupleEqual("przyklad");
    TupleElem elemTupleNotEqual("przyklad2");
    EXPECT_EQ(pattern.matches(elemTupleEqual), false);
    EXPECT_EQ(pattern.matches(elemTupleNotEqual), true);
}

TEST(Tuple, StringGreaterThan) {
    String pattern(">przyklad");
    TupleElem elemTupleGreaterThan("przyklad2");
    TupleElem elemTupleEqual("przyklad");
    TupleElem elemTupleLessThan("przyk");
    EXPECT_EQ(pattern.matches(elemTupleGreaterThan), true);
    EXPECT_EQ(pattern.matches(elemTupleEqual), false);
    EXPECT_EQ(pattern.matches(elemTupleLessThan), false);
}

TEST(Tuple, StringNotGreaterThan) {
    String pattern("<=przyklad");
    TupleElem elemTupleGreaterThan("przyklad2");
    TupleElem elemTupleEqual("przyklad");
    TupleElem elemTupleLessThan("przyk");
    EXPECT_EQ(pattern.matches(elemTupleGreaterThan), false);
    EXPECT_EQ(pattern.matches(elemTupleEqual), true);
    EXPECT_EQ(pattern.matches(elemTupleLessThan), true);
}

TEST(Tuple, StringLessThan) {
    String pattern("<przyklad");
    TupleElem elemTupleGreaterThan("przyklad2");
    TupleElem elemTupleEqual("przyklad");
    TupleElem elemTupleLessThan("przyk");
    EXPECT_EQ(pattern.matches(elemTupleGreaterThan), false);
    EXPECT_EQ(pattern.matches(elemTupleEqual), false);
    EXPECT_EQ(pattern.matches(elemTupleLessThan), true);
}

TEST(Tuple, StringNotLessThan) {
    String pattern(">=przyklad");
    TupleElem elemTupleGreaterThan("przyklad2");
    TupleElem elemTupleEqual("przyklad");
    TupleElem elemTupleLessThan("przyk");
    EXPECT_EQ(pattern.matches(elemTupleGreaterThan), true);
    EXPECT_EQ(pattern.matches(elemTupleEqual), true);
    EXPECT_EQ(pattern.matches(elemTupleLessThan), false);
}

TEST(Tuple, IntAll) {
    Int pattern("*");
    TupleElem elemTuple(4);
    EXPECT_EQ(pattern.matches(elemTuple), true);
}

TEST(Tuple, IntEqual) {
    Int pattern("==4");
    TupleElem elemTupleEqual(4);
    TupleElem elemTupleNotEqual(7);
    EXPECT_EQ(pattern.matches(elemTupleEqual), true);
    EXPECT_EQ(pattern.matches(elemTupleNotEqual), false);
}

TEST(Tuple, IntNotEqual) {
    Int pattern("!=4");
    TupleElem elemTupleEqual(4);
    TupleElem elemTupleNotEqual(7);
    EXPECT_EQ(pattern.matches(elemTupleEqual), false);
    EXPECT_EQ(pattern.matches(elemTupleNotEqual), true);
}

TEST(Tuple, IntGreaterThan) {
    Int pattern(">4");
    TupleElem elemTupleGreaterThan(7);
    TupleElem elemTupleEqual(4);
    TupleElem elemTupleLessThan(2);
    EXPECT_EQ(pattern.matches(elemTupleGreaterThan), true);
    EXPECT_EQ(pattern.matches(elemTupleEqual), false);
    EXPECT_EQ(pattern.matches(elemTupleLessThan), false);
}

TEST(Tuple, IntNotGreaterThan) {
    Int pattern("<=4");
    TupleElem elemTupleGreaterThan(7);
    TupleElem elemTupleEqual(4);
    TupleElem elemTupleLessThan(2);
    EXPECT_EQ(pattern.matches(elemTupleGreaterThan), false);
    EXPECT_EQ(pattern.matches(elemTupleEqual), true);
    EXPECT_EQ(pattern.matches(elemTupleLessThan), true);
}

TEST(Tuple, IntLessThan) {
    Int pattern("<4");
    TupleElem elemTupleGreaterThan(7);
    TupleElem elemTupleEqual(4);
    TupleElem elemTupleLessThan(2);
    EXPECT_EQ(pattern.matches(elemTupleGreaterThan), false);
    EXPECT_EQ(pattern.matches(elemTupleEqual), false);
    EXPECT_EQ(pattern.matches(elemTupleLessThan), true);
}

TEST(Tuple, IntNotLessThan) {
    Int pattern(">=4");
    TupleElem elemTupleGreaterThan(7);
    TupleElem elemTupleEqual(4);
    TupleElem elemTupleLessThan(2);
    EXPECT_EQ(pattern.matches(elemTupleGreaterThan), true);
    EXPECT_EQ(pattern.matches(elemTupleEqual), true);
    EXPECT_EQ(pattern.matches(elemTupleLessThan), false);
}

TEST(Tuple, FloatAll) {
    Float pattern("*");
    TupleElem elemTuple(4.5);
    EXPECT_EQ(pattern.matches(elemTuple), true);
}

TEST(Tuple, FloatEqual) {
    Float pattern("==4.5");
    TupleElem elemTupleEqual(4.5);
    TupleElem elemTupleNotEqual(7.5);
    EXPECT_EQ(pattern.matches(elemTupleEqual), true);
    EXPECT_EQ(pattern.matches(elemTupleNotEqual), false);
}

TEST(Tuple, FloatNotEqual) {
    Float pattern("!=4.5");
    TupleElem elemTupleEqual(4.5);
    TupleElem elemTupleNotEqual(7.5);
    EXPECT_EQ(pattern.matches(elemTupleEqual), false);
    EXPECT_EQ(pattern.matches(elemTupleNotEqual), true);
}

TEST(Tuple, FloatGreaterThan) {
    Float pattern(">4.5");
    TupleElem elemTupleGreaterThan(7.5);
    TupleElem elemTupleEqual(4.5);
    TupleElem elemTupleLessThan(2.2);
    EXPECT_EQ(pattern.matches(elemTupleGreaterThan), true);
    EXPECT_EQ(pattern.matches(elemTupleEqual), false);
    EXPECT_EQ(pattern.matches(elemTupleLessThan), false);
}

TEST(Tuple, FloatNotGreaterThan) {
    Float pattern("<=4.5");
    TupleElem elemTupleGreaterThan(7.5);
    TupleElem elemTupleEqual(4.5);
    TupleElem elemTupleLessThan(2.2);
    EXPECT_EQ(pattern.matches(elemTupleGreaterThan), false);
    EXPECT_EQ(pattern.matches(elemTupleEqual), true);
    EXPECT_EQ(pattern.matches(elemTupleLessThan), true);
}

TEST(Tuple, FloatLessThan) {
    Float pattern("<4.5");
    TupleElem elemTupleGreaterThan(7.5);
    TupleElem elemTupleEqual(4.5);
    TupleElem elemTupleLessThan(2.2);
    EXPECT_EQ(pattern.matches(elemTupleGreaterThan), false);
    EXPECT_EQ(pattern.matches(elemTupleEqual), false);
    EXPECT_EQ(pattern.matches(elemTupleLessThan), true);
}

TEST(Tuple, FloatNotLessThan) {
    Float pattern(">=4.5");
    TupleElem elemTupleGreaterThan(7.5);
    TupleElem elemTupleEqual(4.5);
    TupleElem elemTupleLessThan(2.2);
    EXPECT_EQ(pattern.matches(elemTupleGreaterThan), true);
    EXPECT_EQ(pattern.matches(elemTupleEqual), true);
    EXPECT_EQ(pattern.matches(elemTupleLessThan), false);
}

TEST(Serialization, OneTuplePatternElemInt) {
    Int msg("==123");
    auto bytes = serialize(msg);
    auto c_it = bytes.cbegin();
    auto recv_msg = deserialize(c_it, bytes.cend());
    EXPECT_EQ(recv_msg->GetType(), msg.GetType());
    auto recv_tuple = static_cast<Int*>(recv_msg.get());
    TupleElem elem(123);
    EXPECT_EQ(recv_tuple->matches(elem), true);
    elem = TupleElem(124);
    EXPECT_EQ(recv_tuple->matches(elem), false);
}

TEST(Serialization, OneTuplePatternElemFloat) {
    Float msg("==123.5");
    auto bytes = serialize(msg);
    auto c_it = bytes.cbegin();
    auto recv_msg = deserialize(c_it, bytes.cend());
    EXPECT_EQ(recv_msg->GetType(), msg.GetType());
    auto recv_tuple = static_cast<Float*>(recv_msg.get());
    TupleElem elem(123.5);
    EXPECT_EQ(recv_tuple->matches(elem), true);
    elem = TupleElem(12.2);
    EXPECT_EQ(recv_tuple->matches(elem), false);
}

TEST(Serialization, OneTuplePatternElemString) {
    String msg("==test");
    auto bytes = serialize(msg);
    auto c_it = bytes.cbegin();
    auto recv_msg = deserialize(c_it, bytes.cend());
    EXPECT_EQ(recv_msg->GetType(), msg.GetType());
    auto recv_tuple = static_cast<String*>(recv_msg.get());
    TupleElem elem(std::string("test"));
    EXPECT_EQ(recv_tuple->matches(elem), true);
    elem = TupleElem("test2");
    EXPECT_EQ(recv_tuple->matches(elem), false);
}

TEST(Serialization, OneTuplePatternElemIntAll) {
    Int msg("*");
    auto bytes = serialize(msg);
    auto c_it = bytes.cbegin();
    auto recv_msg = deserialize(c_it, bytes.cend());
    EXPECT_EQ(recv_msg->GetType(), msg.GetType());
    auto recv_tuple = static_cast<Int*>(recv_msg.get());
    TupleElem elem(123);
    EXPECT_EQ(recv_tuple->matches(elem), true);
    elem = TupleElem(124);
    EXPECT_EQ(recv_tuple->matches(elem), true);
}

TEST(Serialization, OneTuplePatternElemFloatAll) {
    Float msg("*");
    auto bytes = serialize(msg);
    auto c_it = bytes.cbegin();
    auto recv_msg = deserialize(c_it, bytes.cend());
    EXPECT_EQ(recv_msg->GetType(), msg.GetType());
    auto recv_tuple = static_cast<Float*>(recv_msg.get());
    TupleElem elem(123.5);
    EXPECT_EQ(recv_tuple->matches(elem), true);
    elem = TupleElem(12.2);
    EXPECT_EQ(recv_tuple->matches(elem), true);
}

TEST(Serialization, OneTuplePatternElemStringAll) {
    String msg("*");
    auto bytes = serialize(msg);
    auto c_it = bytes.cbegin();
    auto recv_msg = deserialize(c_it, bytes.cend());
    EXPECT_EQ(recv_msg->GetType(), msg.GetType());
    auto recv_tuple = static_cast<String*>(recv_msg.get());
    TupleElem elem(std::string("test"));
    EXPECT_EQ(recv_tuple->matches(elem), true);
    elem = TupleElem("test2");
    EXPECT_EQ(recv_tuple->matches(elem), true);
}
