#include "Clove/Platform/Input/Mouse.hpp"
#include "Clove/Platform/Linux/CloveLinux.hpp"

namespace garlic::clove {
    vec2i Mouse::getPosition() const {
        ::Window rootReturn{};
        ::Window childReturn{};
        int rootX{ 0 };//Relative to the root window
        int rootY{ 0 };//Relative to the root window
        int mouseX{ 0 };
        int mouseY{ 0 };
        unsigned int mask{};

        XQueryPointer(display, XDefaultRootWindow(display), &rootReturn, &childReturn, &rootX, &rootY, &mouseX, &mouseY, &mask);

        return { mouseX, mouseY };
    }

    void Mouse::setPosition(vec2i const &position) {
        ::Window const sourceWindow{ 0 };
        ::Window const destWindow{ XDefaultRootWindow(display) };

        int const sourceX{ 0 };
        int const sourceY{ 0 };
        unsigned int const sourceWidth{ 0 };
        unsigned int const sourceHeight{ 0 };

        XWarpPointer(display, sourceWindow, destWindow, sourceX, sourceY, sourceWidth, sourceHeight, position.x, position.y);
    }

    void Mouse::show(bool shouldShow) {
    }
}