/*
 * Project: TinyAd
 * Description: Representation of schedule
 *              used to manage content display times
 * Author: Kamil Przybyła
 * Created: 30.04.2021
 */

#ifndef SCHEDULE_HPP
#define SCHEDULE_HPP

#include <optional>
#include <string>
#include <vector>

#include "config_parser.hpp"
#include "time.hpp"

namespace TinyAd {

class ScheduleParserException : public TinyAdException {
public:
    ScheduleParserException(const std::string& msg) { msg_ = msg; }
};

class ScheduleEntry {
public:
    ScheduleEntry(const std::string& filename)
        : filename_(filename), expiration_time_(), weekdays_(), hour_ranges_() {}
    ScheduleEntry(const std::string& filename, const DateHour& expiration_time)
        : filename_(filename), expiration_time_(expiration_time), weekdays_(), hour_ranges_() {}

    bool hasExpirationTime() const { return expiration_time_.has_value(); }
    bool hasWeekdays() const { return weekdays_.size() > 0; }
    bool hasHourRanges() const { return hour_ranges_.size() > 0; }

    const std::string& getFilename() const { return filename_; }
    const std::optional<DateHour>& getExpirationTime() const { return expiration_time_; }
    const std::vector<WeekdayEnum>& getWeekdays() const { return weekdays_; }
    const std::vector<HourRange>& getHourRanges() const { return hour_ranges_; }

    void setExpirationTime(const DateHour& time) { expiration_time_ = time; }
    void setWeekdays(const std::vector<WeekdayEnum>& weekdays);
    void setHourRanges(const std::vector<HourRange>& hour_ranges);

    void addWeekday(WeekdayEnum weekday);
    void addHourRanges(const HourRange& hour_range);

    std::string toString() const;

private:
    std::string filename_;
    std::optional<DateHour> expiration_time_;
    std::vector<WeekdayEnum> weekdays_;
    std::vector<HourRange> hour_ranges_;
};

class Schedule {
public:
    Schedule() : Schedule("") {}
    Schedule(const std::string& filename);

    std::optional<ScheduleEntry> getValidEntry() const;
    const std::vector<ScheduleEntry>& getEntries() const { return entries_; }

    void addEntry(const ScheduleEntry& entry);

    std::string toString() const;

private:
    static ScheduleEntry makeEntry(const ConfigBlock& config_block, const std::string& path = "");
    static std::vector<HourRange> parseHoursList(const ConfigValue& value);
    static std::vector<WeekdayEnum> parseWeekdaysList(const ConfigValue& value);

    std::string path_;
    std::vector<ScheduleEntry> entries_;
};

}  // namespace TinyAd

#endif