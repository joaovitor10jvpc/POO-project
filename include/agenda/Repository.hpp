#pragma once
#include <vector>
#include <memory>
#include <unordered_map>
#include <functional>
#include <optional>
#include <agenda/DomainTypes.hpp>

namespace agenda {

// Generic in-memory repository (template usage requirement)
// T must have Id id() const

template <typename T>
class Repository {
    std::unordered_map<Id, T> items_;
public:
    bool add(const T& item) { return items_.emplace(item.id(), item).second; }
    std::optional<T> get(Id id) const {
        if(auto it = items_.find(id); it != items_.end()) return it->second;
        return std::nullopt;
    }
    template <typename Pred>
    std::vector<T> findIf(Pred&& p) const {
        std::vector<T> out; out.reserve(items_.size());
        for(auto& [id, v] : items_) if(p(v)) out.push_back(v);
        return out;
    }
    std::vector<T> all() const {
        std::vector<T> out; out.reserve(items_.size());
        for(auto& kv : items_) out.push_back(kv.second);
        return out;
    }
};

} // namespace agenda
