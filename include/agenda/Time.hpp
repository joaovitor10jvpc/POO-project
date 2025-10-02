#pragma once
#include <string>
#include <stdexcept>
#include <iomanip>
#include <sstream>
#include <compare>

namespace agenda {

class Time {
    int hour_; // 0-23
    int minute_; // 0-59
public:
    Time(int hour = 0, int minute = 0) : hour_(hour), minute_(minute) {
        if(hour < 0 || hour > 23 || minute < 0 || minute > 59) {
            throw std::invalid_argument("Invalid time");
        }
    }
    int hour() const { return hour_; }
    int minute() const { return minute_; }

    std::strong_ordering operator<=>(const Time& other) const = default;
    bool operator==(const Time&) const = default;
    bool operator<(const Time& other) const {
        if (hour_ != other.hour_) return hour_ < other.hour_;
        return minute_ < other.minute_;
    }

    std::string toString() const {
        std::ostringstream oss; oss << std::setw(2) << std::setfill('0') << hour_ << ':'
                                    << std::setw(2) << std::setfill('0') << minute_;
        return oss.str();
    }
};

} // namespace agenda
