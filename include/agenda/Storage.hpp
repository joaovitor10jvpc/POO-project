#pragma once
#include <string>
#include <vector>
#include <agenda/Event.hpp>
#include <agenda/ICalendarSerializer.hpp>

namespace agenda {

class IStorage {
public:
    virtual ~IStorage() = default;
    virtual void save(const std::string& path, const std::vector<Event>& events, const ICalendarSerializer& s) = 0;
    virtual std::vector<Event> load(const std::string& path, const ICalendarSerializer& s) = 0;
};

} // namespace agenda
