#pragma once
#include <memory>
#include <agenda/gui/IView.hpp>

namespace agenda { class Calendar; }

namespace agenda::gui {

class IGUIFactory {
public:
    virtual ~IGUIFactory() = default;
    virtual std::unique_ptr<ICalendarView> createCalendarView(Calendar& model) = 0;
};

} // namespace agenda::gui
