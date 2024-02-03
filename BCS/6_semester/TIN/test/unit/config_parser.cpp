/*
 * Project: TinyAd
 * Description: Tests for generic key-value config parser
 * Author: Kamil Przybyła
 * Created: 19.05.2021
 */

#include "config_parser.hpp"

#include <gtest/gtest.h>

#include <sstream>

using namespace TinyAd;

TEST(ConfigParser, ConfigValueBasic) {
    const ConfigValue value{"test"};

    EXPECT_EQ(value.getString(), "test");
}

TEST(ConfigParser, ConfigValueList) {
    const ConfigValue value{"[elem1][elem2][elem3]"};
    const ConfigValue valueWrong{"elem1][elem2][elem3]"};
    const ConfigValue valueWrong2{"[elem1][elem2[elem3]"};
    const ConfigValue valueWrong3{"[elem1][elem2][elem3"};
    const ConfigValue valueWrong4{"[el[em1][elem2][elem3]"};

    EXPECT_THROW(valueWrong.parseList(), ConfigParserException);
    EXPECT_THROW(valueWrong2.parseList(), ConfigParserException);
    EXPECT_THROW(valueWrong3.parseList(), ConfigParserException);
    EXPECT_THROW(valueWrong4.parseList(), ConfigParserException);

    const auto list = value.parseList();
    EXPECT_EQ(list.size(), 3);
    EXPECT_EQ(list[0], "elem1");
    EXPECT_EQ(list[1], "elem2");
    EXPECT_EQ(list[2], "elem3");
}

TEST(ConfigParser, ConfigBlock) {
    const std::unordered_map<std::string, ConfigValue> pairs{{"key1", ConfigValue{"value1"}},
                                                             {"key2", ConfigValue{"value2"}}};

    const ConfigBlock block1{};
    const ConfigBlock block2{pairs};

    const auto should_be_empty = block1.tryGetValue("anything");
    EXPECT_FALSE(should_be_empty.has_value());

    const auto value1 = block2.tryGetValue("key1");
    const auto value2 = block2.tryGetValue("key2");

    EXPECT_TRUE(value1.has_value());
    EXPECT_TRUE(value2.has_value());

    EXPECT_EQ(value1.value().getString(), "value1");
    EXPECT_EQ(value2.value().getString(), "value2");
}

TEST(ConfigParser, ConfigBlockAddKeyValue) {
    ConfigBlock block1{};

    block1.addKeyValue("new_key", "new_value");
    const auto new_value = block1.tryGetValue("new_key");

    EXPECT_TRUE(new_value.has_value());
    EXPECT_EQ(new_value.value().getString(), "new_value");
}

TEST(ConfigParser, ConfigAddBlock) {
    Config config{};

    EXPECT_EQ(config.blocks.size(), 0);

    const std::unordered_map<std::string, ConfigValue> pairs{{"key1", ConfigValue{"value1"}},
                                                             {"key2", ConfigValue{"value2"}}};

    const ConfigBlock block1{};
    const ConfigBlock block2{pairs};

    config.addBlock(block1);
    config.addBlock(block2);

    EXPECT_EQ(config.blocks.size(), 2);
    EXPECT_EQ(config.blocks[1].tryGetValue("key2").value().getString(), "value2");
}

TEST(ConfigParser, ParseWrongConfigThrows) {
    const std::string conf =
        R"(key1=value
key3value3)";

    std::stringstream stream{conf};
    ConfigParser parser{stream};

    EXPECT_THROW(parser.parseConfig(), ConfigParserException);
}

TEST(ConfigParser, ParseConfigSingleBlock) {
    const std::string conf =
        R"(key1=value
key2=[item1][item2]
key3=value3)";

    std::stringstream stream{conf};
    ConfigParser parser{stream};

    const Config config = parser.parseConfig();

    EXPECT_EQ(config.blocks.size(), 1);

    const auto& block = config.blocks[0];
    EXPECT_EQ(block.tryGetValue("key1").value().getString(), "value");
    EXPECT_EQ(block.tryGetValue("key2").value().getString(), "[item1][item2]");
    EXPECT_EQ(block.tryGetValue("key3").value().getString(), "value3");
}

TEST(ConfigParser, ParseConfigIgnoreEmptyLines) {
    const std::string conf =
        R"(key1=value
key2=[item1][item2]


key3=value3



)";

    std::stringstream stream{conf};
    ConfigParser parser{stream};

    const Config config = parser.parseConfig();

    EXPECT_EQ(config.blocks.size(), 1);

    const auto& block = config.blocks[0];
    EXPECT_EQ(block.tryGetValue("key1").value().getString(), "value");
    EXPECT_EQ(block.tryGetValue("key2").value().getString(), "[item1][item2]");
    EXPECT_EQ(block.tryGetValue("key3").value().getString(), "value3");
}

TEST(ConfigParser, ParseConfigMultipleBlocks) {
    const std::string conf =
        R"(key1=value
key2=[item1][item2]
key3=value3
---
key1=val
key2=val2
)";

    std::stringstream stream{conf};
    ConfigParser parser{stream};

    const Config config = parser.parseConfig();

    EXPECT_EQ(config.blocks.size(), 2);

    const auto& block1 = config.blocks[0];
    EXPECT_EQ(block1.tryGetValue("key1").value().getString(), "value");
    EXPECT_EQ(block1.tryGetValue("key2").value().getString(), "[item1][item2]");
    EXPECT_EQ(block1.tryGetValue("key3").value().getString(), "value3");

    const auto& block2 = config.blocks[1];
    EXPECT_EQ(block2.tryGetValue("key1").value().getString(), "val");
    EXPECT_EQ(block2.tryGetValue("key2").value().getString(), "val2");
}