#pragma once
#include <agenda/Date.hpp>
#include <agenda/Time.hpp>
#include <compare>

namespace agenda {

class DateTime {
    Date date_;
    Time time_;
public:
    DateTime() = default;
    DateTime(const Date& d, const Time& t) : date_(d), time_(t) {}
    const Date& date() const { return date_; }
    const Time& time() const { return time_; }
    std::strong_ordering operator<=>(const DateTime& other) const {
        if(auto cmp = date_ <=> other.date_; cmp != 0) return cmp;
        return time_ <=> other.time_;
    }
};

} // namespace agenda
