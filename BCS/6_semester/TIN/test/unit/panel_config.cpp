/*
 * Project: TinyAd
 * Description: Tests for panel config parser
 * Author: Kamil Przybyła
 * Created: 19.05.2021
 */

#include "panel_config.hpp"

#include <gtest/gtest.h>

using namespace TinyAd;

TEST(PanelConfig, ParseFile) {
    const auto conf = PanelConfig::fromFile("./test/unit/data/panel.conf");

    EXPECT_EQ(conf.getName(), "Lobby Front");
    EXPECT_EQ(conf.getServerIp(), "::1");
    EXPECT_EQ(conf.getNumRetries(), 5);
    EXPECT_EQ(conf.getTimeout(), 1);

    const auto& tags = conf.getTags();
    EXPECT_EQ(tags.size(), 3);
    EXPECT_EQ(tags[0], "oled");
    EXPECT_EQ(tags[1], "huge");
    EXPECT_EQ(tags[2], "orange apricot");
}

TEST(PanelConfig, AddAndCheckTags) {
    auto conf = PanelConfig::fromFile("./test/unit/data/panel.conf");

    EXPECT_TRUE(conf.hasTag("oled"));
    EXPECT_TRUE(conf.hasTag("huge"));
    EXPECT_TRUE(conf.hasTag("orange apricot"));
    EXPECT_FALSE(conf.hasTag("brand new tag"));

    conf.addTag("brand new tag");
    EXPECT_TRUE(conf.hasTag("brand new tag"));
}