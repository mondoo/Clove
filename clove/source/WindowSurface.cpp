#include "Clove/WindowSurface.hpp"

#include <Clove/Platform/Window.hpp>

namespace garlic::clove {
    WindowSurface::WindowSurface(std::unique_ptr<Window> window)
        : window{ std::move(window) } {
    }

    WindowSurface::WindowSurface(WindowSurface &&other) noexcept = default;

    WindowSurface &WindowSurface::operator=(WindowSurface &&other) noexcept = default;

    WindowSurface::~WindowSurface() = default;

    void WindowSurface::processInput() {
        window->processInput();
    }

    Keyboard &WindowSurface::getKeyboard() {
        return window->getKeyboard();
    }

    Mouse &WindowSurface::getMouse() {
        return window->getMouse();
    }
}