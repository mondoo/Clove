#include "Clove/Platform/Input/Mouse.hpp"

#include "Clove/Platform/Linux/CloveLinux.hpp"

namespace garlic::clove {
    vec2i Mouse::getPosition() const {
        //XQueryPointer
        return {0, 0};
    }

    void Mouse::setPosition(vec2i const &position) {
        ::Window const sourceWindow{ 0 };
        ::Window const destWindow{ window };

        int const sourceX{ 0 };
        int const sourceY{ 0 };
        unsigned int const sourceWidth{ 0 };
        unsigned int const sourceHeight{ 0 };

        XWarpPointer(display, sourceWindow, destWindow, sourceX, sourceY, sourceWidth, sourceHeight, position.x, position.y);
    }

    void Mouse::show(bool shouldShow) {
        
    }
}