#pragma once
#include <agenda/Event.hpp>
#include <agenda/Repository.hpp>
#include <agenda/Observer.hpp>
#include <agenda/UndoRedo.hpp>
#include <agenda/Exceptions.hpp>
#include <functional>

namespace agenda {

class Calendar : public Subject {
    Repository<Event> events_;
public:
    bool addEvent(const Event& e) {
        bool ok = events_.add(e);
        if(ok) notifyObservers();
        return ok;
    }
    std::vector<Event> events() const { return events_.all(); }
};

} // namespace agenda
