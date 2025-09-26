#pragma once
#include <string>
#include <memory>
#include <vector>
#include <optional>
#include <agenda/Date.hpp>

namespace agenda {

enum class RecurrenceFrequency { Daily, Weekly, Monthly, Yearly };

class RecurrenceRule {
    RecurrenceFrequency frequency_ {RecurrenceFrequency::Daily};
    int interval_ {1}; // every N units
    std::optional<int> count_; // stop after count occurrences
    std::optional<Date> until_; // or until date
public:
    RecurrenceRule() = default;
    RecurrenceRule(RecurrenceFrequency f, int interval = 1) : frequency_(f), interval_(interval) {}
    RecurrenceFrequency frequency() const { return frequency_; }
    int interval() const { return interval_; }
    const std::optional<int>& count() const { return count_; }
    const std::optional<Date>& until() const { return until_; }
    void setCount(int c) { count_ = c; }
    void setUntil(const Date& d) { until_ = d; }
};

} // namespace agenda
