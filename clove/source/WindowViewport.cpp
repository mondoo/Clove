#include "Clove/WindowViewport.hpp"

#include <Clove/Platform/Window.hpp>

namespace garlic::clove {
    WindowViewport::WindowViewport(std::unique_ptr<Window> window)
        : window{ std::move(window) } {
    }

    WindowViewport::WindowViewport(WindowViewport &&other) noexcept = default;

    WindowViewport &WindowViewport::operator=(WindowViewport &&other) noexcept = default;

    WindowViewport::~WindowViewport();

    void WindowViewport::processInput() {
        window->processInput();
    }

    Keyboard &WindowViewport::getKeyboard() {
        return window->getKeyboard();
    }

    Mouse &WindowViewport::getMouse() {
        return window->getMouse();
    }
}