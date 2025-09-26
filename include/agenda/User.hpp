#pragma once
#include <string>
#include <agenda/DomainTypes.hpp>

namespace agenda {

class User {
    Id id_ {0};
    std::string name_;
public:
    User() = default;
    User(Id id, std::string name) : id_(id), name_(std::move(name)) {}
    Id id() const { return id_; }
    const std::string& name() const { return name_; }
};

} // namespace agenda
