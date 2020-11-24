#include "Clove/Platform/Windows/WindowsPlatform.hpp"

#include "Clove/Platform/Windows/WindowsWindow.hpp"

namespace garlic::clove {
    WindowsPlatform::WindowsPlatform() = default;

    WindowsPlatform::WindowsPlatform(WindowsPlatform &&other) noexcept = default;

    WindowsPlatform &WindowsPlatform::operator=(WindowsPlatform &&other) noexcept = default;

    WindowsPlatform::~WindowsPlatform() = default;

    std::shared_ptr<Window> WindowsPlatform::createWindow(WindowDescriptor const &descriptor) {
        return std::make_shared<WindowsWindow>(descriptor);
    }

    std::shared_ptr<Window> WindowsPlatform::createChildWindow(Window const &parentWindow, vec2i const &position, vec2i const &size) {
        return std::make_shared<WindowsWindow>(parentWindow, position, size);
    }
}