#pragma once

#include <Clove/Platform/Input/Keyboard.hpp>
#include <Clove/Platform/Input/Mouse.hpp>
#include <variant>

namespace garlic::clove {
    enum class InputEventType {
        Keyboard,
        Mouse
    };

    struct InputEvent {
        std::variant<Keyboard::Event, Mouse::Event> event;
        InputEventType eventType;
    };
}