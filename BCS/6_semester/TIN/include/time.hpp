/*
 * Project: TinyAd
 * Description: Various objects for representing time
 * Author: Kamil Przybyła
 * Created: 01.05.2021
 */

#ifndef TIME_HPP
#define TIME_HPP

#include <iostream>
#include <string>

#include "exceptions.hpp"

namespace TinyAd {

class WeekdayException : public TinyAdException {
public:
    WeekdayException() { msg_ = "Improper weekday number!"; }
};

class HourException : public TinyAdException {
public:
    HourException(const std::string& msg) { msg_ = msg; }
};

class HourRangeException : public TinyAdException {
public:
    HourRangeException() { msg_ = "Improper hour range!"; }
};

class DateException : public TinyAdException {
public:
    DateException(const std::string& msg) { msg_ = msg; }
};

class DateHourException : public TinyAdException {
public:
    DateHourException() { msg_ = "Improper format!"; }
};

enum class WeekdayEnum { Monday = 1, Tuesday, Wednesday, Thursday, Friday, Saturday, Sunday };

class Weekday {
public:
    Weekday() = delete;

    static WeekdayEnum fromNumber(unsigned short);
    static std::string toString(WeekdayEnum weekday);
    static WeekdayEnum now();
};

class Hour {
public:
    Hour(unsigned short hours, unsigned short minutes);

    // Note: Assumed format HH:MM
    Hour(const std::string& str);

    static Hour now();

    std::string toString() const;

    unsigned short getHours() const { return hours_; };
    unsigned short getMinutes() const { return minutes_; };

    bool operator==(const Hour& other) const;
    bool operator!=(const Hour& other) const;
    bool operator<(const Hour& other) const;
    bool operator<=(const Hour& other) const;
    bool operator>(const Hour& other) const;
    bool operator>=(const Hour& other) const;

    explicit operator std::string() const { return toString(); }
    friend std::ostream& operator<<(std::ostream& os, const Hour& hour);

private:
    unsigned short hours_;
    unsigned short minutes_;
};

inline std::ostream& operator<<(std::ostream& os, const Hour& hour) {
    os << hour.toString();
    return os;
}

class HourRange {
public:
    HourRange(const Hour& min, const Hour& max);

    const Hour& min() const { return min_; }
    const Hour& max() const { return max_; }

    bool contains(const Hour& hour) const;

    bool operator==(const HourRange& other) const;
    bool operator!=(const HourRange& other) const;

private:
    Hour min_;
    Hour max_;
};

class Date {
public:
    using date_type = unsigned int;

    Date(date_type year, date_type month, date_type day);

    // Note: Assumed format YYYY.MM.DD
    Date(const std::string& str);

    static Date now();

    std::string toString() const;

    date_type getYear() const { return year_; };
    date_type getMonth() const { return month_; };
    date_type getDay() const { return day_; };

    bool operator==(const Date& other) const;
    bool operator!=(const Date& other) const;
    bool operator<(const Date& other) const;
    bool operator<=(const Date& other) const;
    bool operator>(const Date& other) const;
    bool operator>=(const Date& other) const;

    explicit operator std::string() const { return toString(); }
    friend std::ostream& operator<<(std::ostream& os, const Date& date);

private:
    bool verifyDate() const;

    static bool isLeapYear(date_type year);
    static date_type getDaysInMonth(date_type month, date_type year);

    // Note: AD dates only with year range 0-999
    date_type year_;
    date_type month_;
    date_type day_;
};

inline std::ostream& operator<<(std::ostream& os, const Date& date) {
    os << date.toString();
    return os;
}

class DateHour {
public:
    DateHour(const Date& date, const Hour& hour) : date_(date), hour_(hour) {}

    // Note: Assumed format YYYY.MM.DD HH:MM
    static DateHour fromString(const std::string& str);
    static DateHour now();

    std::string toString() const;

    const Date& getDate() const { return date_; };
    const Hour& getHour() const { return hour_; };

    bool operator==(const DateHour& other) const;
    bool operator!=(const DateHour& other) const;
    bool operator<(const DateHour& other) const;
    bool operator<=(const DateHour& other) const;
    bool operator>(const DateHour& other) const;
    bool operator>=(const DateHour& other) const;

    explicit operator std::string() const { return toString(); }
    friend std::ostream& operator<<(std::ostream& os, const DateHour& time);

private:
    Date date_;
    Hour hour_;
};

inline std::ostream& operator<<(std::ostream& os, const DateHour& time) {
    os << time.toString();
    return os;
}

}  // namespace TinyAd

#endif