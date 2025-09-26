#pragma once
#include <string>
#include <vector>

namespace agenda {

class Event; // forward declaration

// Serializer interface (Strategy) separada para permitir múltiplos formatos
class ICalendarSerializer {
public:
    virtual ~ICalendarSerializer() = default;
    virtual std::string serialize(const std::vector<Event>& events) const = 0;
    virtual std::vector<Event> deserialize(const std::string& data) const = 0;
};

} // namespace agenda
