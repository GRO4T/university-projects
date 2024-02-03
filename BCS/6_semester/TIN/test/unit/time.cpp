/*
 * Project: TinyAd
 * Description: Unit tests for time management classes
 * Author: Kamil Przybyła
 * Created: 01.05.2021
 */

#include "time.hpp"

#include <gtest/gtest.h>

using namespace TinyAd;

TEST(Time, WeekdayFromNumber) {
    EXPECT_THROW(Weekday::fromNumber(0), WeekdayException);
    EXPECT_THROW(Weekday::fromNumber(8), WeekdayException);

    EXPECT_EQ(Weekday::fromNumber(1), WeekdayEnum::Monday);
    EXPECT_EQ(Weekday::fromNumber(2), WeekdayEnum::Tuesday);
    EXPECT_EQ(Weekday::fromNumber(3), WeekdayEnum::Wednesday);
    EXPECT_EQ(Weekday::fromNumber(4), WeekdayEnum::Thursday);
    EXPECT_EQ(Weekday::fromNumber(5), WeekdayEnum::Friday);
    EXPECT_EQ(Weekday::fromNumber(6), WeekdayEnum::Saturday);
    EXPECT_EQ(Weekday::fromNumber(7), WeekdayEnum::Sunday);
}

TEST(Time, HourBasicCtor) {
    EXPECT_THROW(Hour(24, 30), HourException);
    EXPECT_THROW(Hour(23, 60), HourException);

    const Hour hour{10, 31};

    EXPECT_EQ(hour.getHours(), 10);
    EXPECT_EQ(hour.getMinutes(), 31);
}

TEST(Time, HourStringCtor) {
    EXPECT_THROW(Hour("2f:23"), HourException);
    EXPECT_THROW(Hour("21:2f"), HourException);
    EXPECT_THROW(Hour("231:121"), HourException);
    EXPECT_THROW(Hour("12a32"), HourException);
    EXPECT_THROW(Hour("24:10"), HourException);
    EXPECT_THROW(Hour("10:60"), HourException);

    const Hour hour{"14:42"};

    EXPECT_EQ(hour.getHours(), 14);
    EXPECT_EQ(hour.getMinutes(), 42);
}

TEST(Time, HourToString) {
    const Hour hourA{23, 10};
    const Hour hourB{1, 11};
    const Hour hourC{1, 2};

    EXPECT_EQ(hourA.toString(), "23:10");
    EXPECT_EQ(hourB.toString(), "01:11");
    EXPECT_EQ(hourC.toString(), "01:02");
}

TEST(Time, HourStringOperator) {
    const Hour hourA{23, 10};
    const Hour hourB{1, 11};
    const Hour hourC{1, 2};

    EXPECT_EQ((std::string)hourA, "23:10");
    EXPECT_EQ((std::string)hourB, "01:11");
    EXPECT_EQ((std::string)hourC, "01:02");
}

TEST(Time, HourComparisons) {
    const Hour hourA{21, 10};
    const Hour hourB{21, 11};
    const Hour hourC{21, 9};
    const Hour hourD{20, 10};
    const Hour hourE{22, 10};
    const Hour hourF{21, 10};

    EXPECT_FALSE(hourA == hourB);
    EXPECT_FALSE(hourA == hourC);
    EXPECT_FALSE(hourA == hourD);
    EXPECT_FALSE(hourA == hourE);

    EXPECT_FALSE(hourA != hourF);

    EXPECT_FALSE(hourA < hourC);
    EXPECT_FALSE(hourA < hourD);
    EXPECT_FALSE(hourA < hourF);

    EXPECT_FALSE(hourA > hourB);
    EXPECT_FALSE(hourA > hourE);

    EXPECT_TRUE(hourA == hourF);
    EXPECT_TRUE(hourA <= hourF);
    EXPECT_TRUE(hourA >= hourF);

    EXPECT_TRUE(hourA != hourB);
    EXPECT_TRUE(hourA < hourB);
    EXPECT_TRUE(hourA <= hourB);
    EXPECT_TRUE(hourA > hourC);
    EXPECT_TRUE(hourA >= hourC);
}

TEST(Time, HourRangeCtor) {
    EXPECT_THROW(HourRange(Hour(10, 20), Hour(9, 10)), HourRangeException);

    const HourRange range{Hour(12, 30), Hour(14, 20)};

    EXPECT_EQ(range.min(), Hour(12, 30));
    EXPECT_EQ(range.max(), Hour(14, 20));
}

TEST(Time, HourRangeContains) {
    const HourRange range{Hour(12, 30), Hour(14, 23)};

    EXPECT_TRUE(range.contains(Hour(12, 30)));
    EXPECT_TRUE(range.contains(Hour(14, 23)));
    EXPECT_TRUE(range.contains(Hour(12, 36)));
    EXPECT_TRUE(range.contains(Hour(14, 22)));
    EXPECT_TRUE(range.contains(Hour(13, 30)));

    EXPECT_FALSE(range.contains(Hour(12, 29)));
    EXPECT_FALSE(range.contains(Hour(15, 29)));
}

TEST(Time, HourRangeCompare) {
    const HourRange range1{Hour(12, 30), Hour(14, 23)};
    const HourRange range2{Hour(13, 31), Hour(14, 23)};
    const HourRange range3{Hour(12, 30), Hour(15, 23)};
    const HourRange range4{Hour(12, 30), Hour(14, 23)};

    EXPECT_TRUE(range1 != range2);
    EXPECT_TRUE(range1 != range3);
    EXPECT_TRUE(range1 == range4);

    EXPECT_FALSE(range1 == range2);
    EXPECT_FALSE(range1 == range3);
    EXPECT_FALSE(range1 != range4);
}

TEST(Time, DateBasicCtor) {
    EXPECT_THROW(Date(10000, 2, 12), DateException);
    EXPECT_THROW(Date(1999, 13, 12), DateException);
    EXPECT_THROW(Date(2012, 1, 32), DateException);
    EXPECT_THROW(Date(2021, 6, 31), DateException);

    EXPECT_THROW(Date(2012, 2, 30), DateException);
    EXPECT_THROW(Date(2013, 2, 29), DateException);

    EXPECT_THROW(Date(2300, 2, 29), DateException);

    EXPECT_NO_THROW(Date(2012, 2, 29));
    EXPECT_NO_THROW(Date(2000, 2, 29));

    const Date date{2021, 5, 4};
    EXPECT_EQ(date.getYear(), 2021);
    EXPECT_EQ(date.getMonth(), 5);
    EXPECT_EQ(date.getDay(), 4);
}

TEST(Time, DateStringCtor) {
    EXPECT_THROW(Date("2020.1.21"), DateException);
    EXPECT_THROW(Date("12d0.01.23"), DateException);
    EXPECT_THROW(Date("1200.0f.23"), DateException);
    EXPECT_THROW(Date("1200.01.f3"), DateException);
    EXPECT_THROW(Date("1233:10.10"), DateException);
    EXPECT_THROW(Date("2020.01a10"), DateException);

    const Date date{"2021.04.12"};
    EXPECT_EQ(date.getYear(), 2021);
    EXPECT_EQ(date.getMonth(), 4);
    EXPECT_EQ(date.getDay(), 12);
}

TEST(Time, DateToString) {
    const Date dateA{2021, 5, 4};
    const Date dateB{221, 11, 5};
    const Date dateC{21, 3, 1};
    const Date dateD{2, 4, 22};

    EXPECT_EQ(dateA.toString(), "2021.05.04");
    EXPECT_EQ(dateB.toString(), "0221.11.05");
    EXPECT_EQ(dateC.toString(), "0021.03.01");
    EXPECT_EQ(dateD.toString(), "0002.04.22");
}

TEST(Time, DateStringOperator) {
    const Date dateA{2021, 5, 4};
    const Date dateB{221, 11, 5};
    const Date dateC{21, 3, 1};
    const Date dateD{2, 4, 22};

    EXPECT_EQ((std::string)dateA, "2021.05.04");
    EXPECT_EQ((std::string)dateB, "0221.11.05");
    EXPECT_EQ((std::string)dateC, "0021.03.01");
    EXPECT_EQ((std::string)dateD, "0002.04.22");
}

TEST(Time, DateComparisons) {
    const Date dateA{2018, 5, 16};
    const Date dateB{2021, 5, 16};
    const Date dateC{2017, 5, 16};
    const Date dateD{2018, 4, 16};
    const Date dateE{2018, 5, 17};
    const Date dateF{2018, 5, 16};

    EXPECT_TRUE(dateA != dateB);
    EXPECT_TRUE(dateA < dateB);
    EXPECT_TRUE(dateA <= dateB);
    EXPECT_TRUE(dateA > dateC);
    EXPECT_TRUE(dateA >= dateC);
    EXPECT_TRUE(dateA > dateD);
    EXPECT_TRUE(dateA < dateE);
    EXPECT_TRUE(dateA == dateF);
    EXPECT_TRUE(dateA >= dateF);
    EXPECT_TRUE(dateA <= dateF);
}

TEST(Time, DateHourCtor) {
    const DateHour time{Date{2012, 11, 10}, Hour{3, 10}};

    EXPECT_EQ(time.getDate(), Date(2012, 11, 10));
    EXPECT_EQ(time.getHour(), Hour(3, 10));
}

TEST(Time, DateHourFromString) {
    const auto time = DateHour::fromString("2012.11.10 03:10");

    EXPECT_EQ(time.getDate(), Date(2012, 11, 10));
    EXPECT_EQ(time.getHour(), Hour(3, 10));
}

TEST(Time, DateHourToString) {
    const DateHour time{Date{2014, 5, 16}, Hour{12, 15}};

    EXPECT_EQ(time.toString(), "2014.05.16 12:15");
}

TEST(Time, DateHourStringOperator) {
    const DateHour time{Date{2015, 11, 5}, Hour{12, 1}};

    EXPECT_EQ((std::string)time, "2015.11.05 12:01");
}

TEST(Time, DateHourComparisons) {
    const DateHour timeA{Date{2015, 11, 5}, Hour{12, 10}};
    const DateHour timeB{Date{2015, 12, 5}, Hour{12, 10}};
    const DateHour timeC{Date{2015, 11, 1}, Hour{12, 10}};
    const DateHour timeD{Date{2015, 11, 5}, Hour{13, 10}};
    const DateHour timeE{Date{2015, 11, 5}, Hour{11, 10}};
    const DateHour timeF{Date{2015, 11, 5}, Hour{12, 10}};

    EXPECT_TRUE(timeA != timeB);
    EXPECT_TRUE(timeA < timeB);
    EXPECT_TRUE(timeA <= timeB);
    EXPECT_TRUE(timeA > timeC);
    EXPECT_TRUE(timeA >= timeC);
    EXPECT_TRUE(timeA < timeD);
    EXPECT_TRUE(timeA > timeE);
    EXPECT_TRUE(timeA == timeF);
    EXPECT_TRUE(timeA >= timeF);
    EXPECT_TRUE(timeA <= timeF);
}