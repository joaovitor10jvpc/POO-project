#pragma once
#include <stdexcept>
#include <string>

namespace agenda {

class AgendaError : public std::runtime_error {
public:
    explicit AgendaError(const std::string& m) : std::runtime_error(m) {}
};

class NotFoundError : public AgendaError {
public:
    explicit NotFoundError(const std::string& m) : AgendaError(m) {}
};

class ValidationError : public AgendaError {
public:
    explicit ValidationError(const std::string& m) : AgendaError(m) {}
};

} // namespace agenda
