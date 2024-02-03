/*
 * Project: TinyAd
 * Description: Tests for schedule parser
 * Author: Kamil Przybyła
 * Created: 30.04.2021
 */

#include <gtest/gtest.h>

#define private public
#include "schedule.hpp"

using namespace TinyAd;

TEST(ScheduleParser, ParseWeekdaysList) {
    const ConfigValue value{"[1][7]"};
    const auto result = Schedule::parseWeekdaysList(value);

    EXPECT_EQ(result.size(), 2);
    EXPECT_EQ(result[0], WeekdayEnum::Monday);
    EXPECT_EQ(result[1], WeekdayEnum::Sunday);
}

TEST(ScheduleParser, ParseImproperWeekdaysListThrows) {
    const ConfigValue value{"[sfasf][7]"};
    EXPECT_ANY_THROW(Schedule::parseWeekdaysList(value));
}

TEST(ScheduleParser, ParseHoursList) {
    const ConfigValue value{"[11:11-12:10][15:20-15:30]"};
    const auto result = Schedule::parseHoursList(value);

    EXPECT_EQ(result.size(), 2);
    EXPECT_EQ(result[0], HourRange(Hour{11, 11}, Hour{12, 10}));
    EXPECT_EQ(result[1], HourRange(Hour{15, 20}, Hour{15, 30}));
}

TEST(ScheduleParser, ParseImproperHoursListThrows) {
    const ConfigValue value{"[11:11 12:10][15:20-15:30]"};
    EXPECT_THROW(Schedule::parseHoursList(value), ScheduleParserException);
}

TEST(ScheduleParser, ParseScheduleEntryFilenameSufficient) {
    const std::unordered_map<std::string, ConfigValue> pairs{{"file", ConfigValue{"some_file"}}};

    const ConfigBlock block{pairs};
    const auto entry = Schedule::makeEntry(block);

    EXPECT_EQ(entry.getFilename(), "some_file");
    EXPECT_FALSE(entry.hasExpirationTime());
    EXPECT_FALSE(entry.hasHourRanges());
    EXPECT_FALSE(entry.hasWeekdays());
}

TEST(ScheduleParser, ParseScheduleEntryFilenameRequired) {
    const std::unordered_map<std::string, ConfigValue> pairs{
        {"expire", ConfigValue{"2021.05.15 19:30"}}};

    const ConfigBlock block{pairs};
    EXPECT_THROW(Schedule::makeEntry(block), ScheduleParserException);
}

TEST(ScheduleParser, ParseScheduleEntryFull) {
    const std::unordered_map<std::string, ConfigValue> pairs{
        {"file", ConfigValue{"some_file"}},
        {"expire", ConfigValue{"2021.05.15 19:30"}},
        {"weekday", ConfigValue{"[2]"}},
        {"hour", ConfigValue{"[03:10-11:11]"}}};

    const ConfigBlock block{pairs};

    const auto entry = Schedule::makeEntry(block);

    EXPECT_EQ(entry.getFilename(), "some_file");
    EXPECT_TRUE(entry.hasExpirationTime());
    EXPECT_TRUE(entry.hasHourRanges());
    EXPECT_TRUE(entry.hasWeekdays());

    EXPECT_EQ(entry.getExpirationTime().value(), DateHour::fromString("2021.05.15 19:30"));
    EXPECT_EQ(entry.getWeekdays().size(), 1);
    EXPECT_EQ(entry.getWeekdays()[0], WeekdayEnum::Tuesday);
    EXPECT_EQ(entry.getHourRanges().size(), 1);
    EXPECT_EQ(entry.getHourRanges()[0], HourRange(Hour{3, 10}, Hour{11, 11}));
}