/*
 * Project: TinyAd
 * Description: Representation of schedule
 *              used to manage content display times
 * Author: Kamil Przybyła
 * Created: 30.04.2021
 */

#include "schedule.hpp"

#include <fstream>

#include "config_parser.hpp"

namespace TinyAd {

void ScheduleEntry::setWeekdays(const std::vector<WeekdayEnum>& weekdays) { weekdays_ = weekdays; }

void ScheduleEntry::setHourRanges(const std::vector<HourRange>& hour_ranges) {
    hour_ranges_ = hour_ranges;
}

void ScheduleEntry::addWeekday(WeekdayEnum weekday) { weekdays_.emplace_back(weekday); }

void ScheduleEntry::addHourRanges(const HourRange& hour_range) {
    hour_ranges_.emplace_back(hour_range);
}

std::string ScheduleEntry::toString() const {
    std::string str = filename_ + "\n";

    if (hasExpirationTime()) {
        str += "Expires: " + expiration_time_.value().toString() + "\n";
    }

    if (hasWeekdays()) {
        str += "Valid in weekdays:\n";
        for (const auto weekday : weekdays_) {
            str += "\t" + Weekday::toString(weekday) + "\n";
        }
    }

    if (hasHourRanges()) {
        str += "Hour ranges:\n";
        for (const auto range : hour_ranges_) {
            str += "\t" + range.min().toString() + "-" + range.max().toString() + "\n";
        }
    }

    str += "\n";
    return str;
}

Schedule::Schedule(const std::string& filename) : entries_{} {
    const std::string path = filename.substr(0, filename.find_last_of("/") + 1);

    std::ifstream file(filename);
    if (!file.is_open()) throw ScheduleParserException("File " + filename + " not found!");

    ConfigParser parser{file};
    const Config config = parser.parseConfig();
    file.close();

    for (const auto& block : config.blocks) {
        addEntry(makeEntry(block, path));
    }
}

std::optional<ScheduleEntry> Schedule::getValidEntry() const {
    const auto now_time = DateHour::now();
    const auto weekday = Weekday::now();

    for (const auto& entry : entries_) {
        if (entry.hasExpirationTime() && entry.getExpirationTime().value() < now_time) continue;

        // If there is no info about weekdays, then all weekdays are valid
        bool matching_weekday = !entry.hasWeekdays();
        for (const auto& valid_weekday : entry.getWeekdays()) {
            if (weekday == valid_weekday) {
                matching_weekday = true;
                break;
            }
        }

        bool matching_hour = !entry.hasHourRanges();
        for (const auto& valid_hours : entry.getHourRanges()) {
            if (valid_hours.contains(now_time.getHour())) {
                matching_hour = true;
                break;
            }
        }

        if (matching_weekday && matching_hour) return entry;
    }

    return {};
}

void Schedule::addEntry(const ScheduleEntry& entry) { entries_.emplace_back(entry); }

std::string Schedule::toString() const {
    std::string str = "\n";
    for (const auto& entry : entries_) {
        str += entry.toString();
    }
    return str;
}

ScheduleEntry Schedule::makeEntry(const ConfigBlock& config_block, const std::string& path) {
    const auto filename = config_block.tryGetValue("file");
    if (!filename.has_value())
        throw ScheduleParserException("Schedule entry does not contain filename!");

    ScheduleEntry entry{path + filename.value().getString()};

    const auto expire = config_block.tryGetValue("expire");
    const auto weekday = config_block.tryGetValue("weekday");
    const auto hour = config_block.tryGetValue("hour");

    if (expire) {
        const auto expire_time = DateHour::fromString(expire.value().getString());
        entry.setExpirationTime(expire_time);
    }

    if (hour) {
        const auto hours_list = parseHoursList(hour.value());
        entry.setHourRanges(hours_list);
    }

    if (weekday) {
        const auto weekdays_list = parseWeekdaysList(weekday.value());
        entry.setWeekdays(weekdays_list);
    }

    return entry;
}

std::vector<HourRange> Schedule::parseHoursList(const ConfigValue& value) {
    const auto items = value.parseList();

    std::vector<HourRange> ranges{};
    for (const auto& item : items) {
        const auto range_separator_idx = item.find('-');
        if (range_separator_idx == std::string::npos)
            throw ScheduleParserException("Expected '-' separator!");

        const auto min_value = item.substr(0, range_separator_idx);
        const auto max_value = item.substr(range_separator_idx + 1);

        const Hour min_hour{min_value};
        const Hour max_hour{max_value};

        ranges.emplace_back(HourRange{min_hour, max_hour});
    }

    return ranges;
}

std::vector<WeekdayEnum> Schedule::parseWeekdaysList(const ConfigValue& value) {
    const auto items = value.parseList();

    std::vector<WeekdayEnum> weekdays{};
    for (const auto& item : items) {
        const auto weekday_number = std::stoi(item);
        weekdays.emplace_back(Weekday::fromNumber(weekday_number));
    }

    return weekdays;
}

}  // namespace TinyAd
