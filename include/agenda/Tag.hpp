#pragma once
#include <string>
#include <compare>
#include <agenda/DomainTypes.hpp>

namespace agenda {

class Tag {
    Id id_ {0};
    std::string name_;
public:
    Tag() = default;
    Tag(Id id, std::string n) : id_(id), name_(std::move(n)) {}
    Id id() const { return id_; }
    const std::string& name() const { return name_; }
    std::strong_ordering operator<=>(const Tag&) const = default;
};

} // namespace agenda
