/*
 * Project: TinyAd
 * Description: Various objects for representing time
 * Author: Kamil Przybyła
 * Created: 01.05.2021
 */

#include "time.hpp"

#include <chrono>
#include <cstring>
#include <sstream>

namespace TinyAd {

WeekdayEnum Weekday::fromNumber(unsigned short weekday) {
    if (weekday > 0 && weekday <= 7)
        return static_cast<WeekdayEnum>(weekday);
    else
        throw WeekdayException();
}

std::string Weekday::toString(WeekdayEnum weekday) {
    switch (weekday) {
        case WeekdayEnum::Monday:
            return "Monday";
        case WeekdayEnum::Tuesday:
            return "Tuesday";
        case WeekdayEnum::Wednesday:
            return "Wednesday";
        case WeekdayEnum::Thursday:
            return "Thursday";
        case WeekdayEnum::Friday:
            return "Friday";
        case WeekdayEnum::Saturday:
            return "Saturday";
        case WeekdayEnum::Sunday:
            return "Sunday";
    }

    return "unreachable";
}

WeekdayEnum Weekday::now() {
    const auto now = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
    const auto currentTime = std::localtime(&now);

    // Shifts from 0 -> Sunday to 1 -> Monday
    const auto day = ((currentTime->tm_wday + 6) % 7) + 1;

    return static_cast<WeekdayEnum>(day);
}

Hour::Hour(unsigned short hours, unsigned short minutes) : hours_(hours), minutes_(minutes) {
    if (hours > 23) throw HourException("Improper hour: " + std::to_string(hours) + "!");

    if (minutes > 59) throw HourException("Improper minute: " + std::to_string(minutes) + "!");
}

Hour::Hour(const std::string& str) {
    if (str.size() != 5 || str[2] != ':') throw HourException("Improper hour format: " + str + "!");

    if (std::isdigit(str[0]) && std::isdigit(str[1]))
        hours_ = 10 * (str[0] - '0') + str[1] - '0';
    else
        throw HourException("Improper hour format: " + str + "!");

    if (std::isdigit(str[3]) && std::isdigit(str[4]))
        minutes_ = 10 * (str[3] - '0') + str[4] - '0';
    else
        throw HourException("Improper hour format: " + str + "!");

    if (hours_ > 23) throw HourException("Improper hour: " + std::to_string(hours_) + "!");

    if (minutes_ > 59) throw HourException("Improper minute: " + std::to_string(minutes_) + "!");
}

Hour Hour::now() {
    const auto now = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
    const auto currentTime = std::localtime(&now);

    const auto hours = static_cast<unsigned short>(currentTime->tm_hour);
    const auto minutes = static_cast<unsigned short>(currentTime->tm_min);

    return Hour{hours, minutes};
}

std::string Hour::toString() const {
    std::stringstream ss;
    if (hours_ < 10) ss << '0';

    ss << hours_ << ':';

    if (minutes_ < 10) ss << '0';

    ss << minutes_;

    return ss.str();
}

bool Hour::operator==(const Hour& other) const {
    return hours_ == other.hours_ && minutes_ == other.minutes_;
}

bool Hour::operator!=(const Hour& other) const { return !operator==(other); }

bool Hour::operator<(const Hour& other) const {
    if (hours_ < other.hours_)
        return true;
    else if (hours_ == other.hours_ && minutes_ < other.minutes_)
        return true;
    else
        return false;
}

bool Hour::operator<=(const Hour& other) const { return *this < other || *this == other; }

bool Hour::operator>(const Hour& other) const { return !(*this < other) && !(*this == other); }

bool Hour::operator>=(const Hour& other) const { return !(*this < other); }

HourRange::HourRange(const Hour& min, const Hour& max) : min_(min), max_(max) {
    if (max < min) throw HourRangeException();
}

bool HourRange::contains(const Hour& hour) const { return hour >= min_ && hour <= max_; }

bool HourRange::operator==(const HourRange& other) const {
    return min_ == other.min_ && max_ == other.max_;
}
bool HourRange::operator!=(const HourRange& other) const {
    return min_ != other.min_ || max_ != other.max_;
}

Date::Date(date_type year, date_type month, date_type day) : year_(year), month_(month), day_(day) {
    if (month_ == 0 || month_ > 12)
        throw DateException("Improper month: " + std::to_string(month) + "!");

    if (day_ == 0 || day_ > 31) throw DateException("Improper day: " + std::to_string(day) + "!");

    if (!verifyDate()) throw DateException("Incorrect date!");
}

Date::Date(const std::string& str) {
    if (str.size() != 10 || str[4] != '.' || str[7] != '.')
        throw DateException("Improper date format: " + str + "!");

    if (std::isdigit(str[0]) && std::isdigit(str[1]) && std::isdigit(str[2]) &&
        std::isdigit(str[3]))
        year_ = 1000 * (str[0] - '0') + 100 * (str[1] - '0') + 10 * (str[2] - '0') + (str[3] - '0');
    else
        throw DateException("Improper date format: " + str + "!");

    if (std::isdigit(str[5]) && std::isdigit(str[6]))
        month_ = 10 * (str[5] - '0') + str[6] - '0';
    else
        throw DateException("Improper date format: " + str + "!");

    if (std::isdigit(str[8]) && std::isdigit(str[9]))
        day_ = 10 * (str[8] - '0') + str[9] - '0';
    else
        throw DateException("Improper date format: " + str + "!");

    if (!verifyDate()) throw DateException("Incorrect date!");
}

Date Date::now() {
    const auto now = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
    const auto currentTime = std::localtime(&now);

    const auto year = static_cast<date_type>(1900 + currentTime->tm_year);
    const auto month = static_cast<date_type>(currentTime->tm_mon + 1);
    const auto day = static_cast<date_type>(currentTime->tm_mday);

    return Date{year, month, day};
}

std::string Date::toString() const {
    std::stringstream ss;

    if (year_ < 10)
        ss << "000";
    else if (year_ < 100)
        ss << "00";
    else if (year_ < 1000)
        ss << '0';

    ss << year_ << '.';

    if (month_ < 10) ss << '0';

    ss << month_ << '.';

    if (day_ < 10) ss << '0';

    ss << day_;

    return ss.str();
}

bool Date::isLeapYear(date_type year) {
    return year % 4 == 0 && (year % 100 != 0 || year % 400 == 0);
}

Date::date_type Date::getDaysInMonth(date_type month, date_type year) {
    if (month == 2) {
        auto leep = isLeapYear(year);
        return leep ? 29 : 28;
    } else if (((month - 1) % 7) % 2 == 0)
        return 31;
    else
        return 30;
}

bool Date::operator==(const Date& other) const {
    return year_ == other.year_ && month_ == other.month_ && day_ == other.day_;
}

bool Date::operator!=(const Date& other) const { return !(*this == other); }

bool Date::operator<(const Date& other) const {
    if (year_ < other.year_)
        return true;
    else if (year_ == other.year_ && month_ < other.month_)
        return true;
    else if (month_ == other.month_ && day_ < other.day_)
        return true;
    else
        return false;
}

bool Date::operator<=(const Date& other) const { return *this < other || *this == other; }

bool Date::operator>(const Date& other) const { return !(*this < other) && !(*this == other); }

bool Date::operator>=(const Date& other) const { return !(*this < other); }

bool Date::verifyDate() const {
    if (year_ > 9999 || month_ == 0 || month_ > 12 || day_ == 0 || day_ > 31) return false;

    const auto daysInMonth = getDaysInMonth(month_, year_);
    return day_ <= daysInMonth;
}

DateHour DateHour::fromString(const std::string& str) {
    if (str.size() != 16) throw DateHourException();

    const auto dateStr = str.substr(0, 10);
    const auto hourStr = str.substr(11, 5);

    return DateHour(Date(dateStr), Hour(hourStr));
}

DateHour DateHour::now() {
    const auto now = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
    const auto currentTime = std::localtime(&now);

    const auto year = static_cast<Date::date_type>(1900 + currentTime->tm_year);
    const auto month = static_cast<Date::date_type>(currentTime->tm_mon + 1);
    const auto day = static_cast<Date::date_type>(currentTime->tm_mday);

    const auto hours = static_cast<unsigned short>(currentTime->tm_hour);
    const auto minutes = static_cast<unsigned short>(currentTime->tm_min);

    return DateHour{Date{year, month, day}, Hour{hours, minutes}};
}

std::string DateHour::toString() const { return date_.toString() + " " + hour_.toString(); }

bool DateHour::operator==(const DateHour& other) const {
    return date_ == other.date_ && hour_ == other.hour_;
}

bool DateHour::operator!=(const DateHour& other) const { return !operator==(other); }

bool DateHour::operator<(const DateHour& other) const {
    if (date_ < other.date_)
        return true;
    else if (date_ == other.date_ && hour_ < other.hour_)
        return true;
    else
        return false;
}

bool DateHour::operator<=(const DateHour& other) const { return *this < other || *this == other; }

bool DateHour::operator>(const DateHour& other) const {
    return !(*this < other) && !(*this == other);
}

bool DateHour::operator>=(const DateHour& other) const { return !(*this < other); }

}  // namespace TinyAd
