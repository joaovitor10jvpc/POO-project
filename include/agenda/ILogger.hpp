#pragma once
#include <string>
#include <memory>

namespace agenda {

class ILogger {
public:
    virtual ~ILogger() = default;
    virtual void info(const std::string& msg) = 0;
    virtual void warn(const std::string& msg) = 0;
    virtual void error(const std::string& msg) = 0;
};

class NullLogger : public ILogger {
public:
    void info(const std::string& ) override {}
    void warn(const std::string& ) override {}
    void error(const std::string& ) override {}
};

} // namespace agenda
