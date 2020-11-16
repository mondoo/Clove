#pragma once

#include <Clove/Input/Keyboard.hpp>
#include <Clove/Input/Mouse.hpp>
#include <variant>

namespace garlic::inline stem {
    enum class InputEventType {
        Keyboard,
        Mouse
    };

    struct InputEvent {
        std::variant<clv::Keyboard::Event, clv::Mouse::Event> event;
        InputEventType eventType;
    };
}