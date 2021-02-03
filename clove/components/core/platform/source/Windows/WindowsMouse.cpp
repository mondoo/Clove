#include "Clove/Platform/Input/Mouse.hpp"

namespace garlic::clove {
    vec2i Mouse::getPosition() const {
        POINT point{};
        GetCursorPos(&point);

        return { point.x, point.y };
    }
}