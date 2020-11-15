#pragma once

#include "Stem/InputResponse.hpp"

#include <Clove/Input/Keyboard.hpp>
#include <Clove/Input/Mouse.hpp>

namespace garlic::inline stem {
    struct ElementBounds {
        clv::mth::vec2i start;
        clv::mth::vec2i end;
    };
}

namespace garlic::inline stem {
    class InputElement {
        //FUNCTIONS
    public:
        virtual ~InputElement() = default;

        virtual InputResponse onKeyEvent(clv::Keyboard::Event const &event) = 0;
        virtual InputResponse onMouseEvent(clv::Mouse::Event const &event)  = 0;

        virtual ElementBounds getBounds() const = 0;
    };
}