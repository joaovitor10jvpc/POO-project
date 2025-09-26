#pragma once
#include <string>
#include <cstdint>
#include <optional>
#include <vector>
#include <memory>
#include <chrono>

namespace agenda {

using Id = std::uint64_t; // Simple integral id type

struct TimeSpan {
    std::chrono::minutes minutes {0};
};

struct Duration {
    std::chrono::minutes value {0};
    explicit Duration(std::chrono::minutes v = std::chrono::minutes{0}) : value(v) {}
    bool operator==(const Duration& other) const { return value == other.value; }
    bool operator<(const Duration& other) const { return value < other.value; }
};

} // namespace agenda
