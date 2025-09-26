#pragma once
#include <vector>
#include <agenda/Event.hpp>

namespace agenda {

struct EventQueryResult {
    std::vector<Event> events;
};

class EventQueryBuilder {
    // placeholder for fluent interface later
public:
    EventQueryBuilder& byTag(const std::string& /*tagName*/) { return *this; }
    EventQueryBuilder& byDate(const Date& /*date*/) { return *this; }
};

} // namespace agenda
