#pragma once
#include <string>
#include <memory>

namespace agenda::gui {

class IView {
public:
    virtual ~IView() = default;
    virtual void show() = 0;
};

class ICalendarView : public IView {
public:
    enum class Mode { Month, Week, Day };
    virtual void setMode(Mode) = 0;
};

class IInputController {
public:
    virtual ~IInputController() = default;
    virtual void onCreateEvent() = 0;
};

} // namespace agenda::gui
