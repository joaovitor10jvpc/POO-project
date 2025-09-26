#pragma once
#include <string>
#include <vector>
#include <memory>
#include <optional>
#include <agenda/DomainTypes.hpp>
#include <agenda/DateTime.hpp>
#include <agenda/RecurrenceRule.hpp>
#include <agenda/Tag.hpp>
#include <agenda/User.hpp>

namespace agenda {

class Event {
    Id id_ {0};
    std::string title_;
    std::string description_;
    DateTime start_;
    Duration duration_ {std::chrono::minutes{0}};
    std::optional<RecurrenceRule> recurrence_;
    std::vector<Tag> tags_;
    std::vector<Id> participantIds_;
    Id ownerId_ {0};
public:
    Event() = default;
    Event(Id id, std::string title, const DateTime& start, Duration dur, Id owner)
        : id_(id), title_(std::move(title)), start_(start), duration_(dur), ownerId_(owner) {}
    Id id() const { return id_; }
    const std::string& title() const { return title_; }
    const std::string& description() const { return description_; }
    const DateTime& start() const { return start_; }
    Duration duration() const { return duration_; }
    const std::optional<RecurrenceRule>& recurrence() const { return recurrence_; }
    const std::vector<Tag>& tags() const { return tags_; }
    const std::vector<Id>& participants() const { return participantIds_; }
    Id ownerId() const { return ownerId_; }

    void setDescription(std::string d) { description_ = std::move(d); }
    void setRecurrence(const RecurrenceRule& r) { recurrence_ = r; }
    void addTag(const Tag& tag) { tags_.push_back(tag); }
    void addParticipant(Id userId) { participantIds_.push_back(userId); }
};

} // namespace agenda
