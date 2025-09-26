#pragma once
#include <vector>
#include <algorithm>

namespace agenda {

class IObserver {
public:
    virtual ~IObserver() = default;
    virtual void update() = 0;
};

class Subject {
    std::vector<IObserver*> observers_;
public:
    void addObserver(IObserver* o) {
        observers_.push_back(o);
    }
    void removeObserver(IObserver* o) {
        observers_.erase(std::remove(observers_.begin(), observers_.end(), o), observers_.end());
    }
protected:
    void notifyObservers() {
        for(auto* o : observers_) if(o) o->update();
    }
};

} // namespace agenda
