#pragma once
#include <agenda/Storage.hpp>
#include <agenda/ICalendarSerializer.hpp>
#include <stdexcept>

namespace agenda {

class JsonSerializer : public ICalendarSerializer {
public:
    std::string serialize(const std::vector<Event>& /*events*/) const override {
        return "{}"; // placeholder
    }
    std::vector<Event> deserialize(const std::string& /*data*/) const override {
        return {}; // placeholder
    }
};

class FileStorage : public IStorage {
public:
    void save(const std::string& /*path*/, const std::vector<Event>& /*events*/, const ICalendarSerializer& /*s*/) override {
        // placeholder
    }
    std::vector<Event> load(const std::string& /*path*/, const ICalendarSerializer& /*s*/) override {
        return {}; // placeholder
    }
};

} // namespace agenda
