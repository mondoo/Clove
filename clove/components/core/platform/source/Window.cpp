#include "Clove/Platform/Window.hpp"

namespace garlic::clove {
    Window::Window() = default;

    Window::~Window() = default;

    Keyboard &Window::getKeyboard() {
        return keyboard;
    }

    Mouse &Window::getMouse() {
        return mouse;
    }
}