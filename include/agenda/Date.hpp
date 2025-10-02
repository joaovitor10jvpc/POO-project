#pragma once
#include <stdexcept>
#include <string>
#include <sstream>
#include <iomanip>
#include <compare>

namespace agenda {

class Date {
    int year_ {1970};
    int month_ {1}; // 1-12
    int day_ {1};   // 1-31 simplistic
public:
    Date() = default;
    Date(int y, int m, int d) : year_(y), month_(m), day_(d) {
        if(m < 1 || m > 12 || d < 1 || d > 31) {
            throw std::invalid_argument("Invalid date");
        }
    }
    int year() const { return year_; }
    int month() const { return month_; }
    int day() const { return day_; }

    // C++20 three-way comparison (synthesizes >, >=, <= in most compilers)
    std::strong_ordering operator<=>(const Date&) const = default;
    // Provide explicit operators to avoid issues with some toolchains
    bool operator==(const Date&) const = default;
    bool operator<(const Date& other) const {
        if (year_ != other.year_) return year_ < other.year_;
        if (month_ != other.month_) return month_ < other.month_;
        return day_ < other.day_;
    }

    std::string toString() const {
        std::ostringstream oss; oss << year_ << '-'
            << std::setw(2) << std::setfill('0') << month_ << '-'
            << std::setw(2) << std::setfill('0') << day_;
        return oss.str();
    }
};

} // namespace agenda
