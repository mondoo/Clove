#include "Clove/Platform/Platform.hpp"

#include "Clove/Platform/Windows/WindowsWindow.hpp"

namespace garlic::clove {
    std::unique_ptr<Window> Platform::createWindow(WindowDescriptor const &descriptor) {
        return std::make_unique<WindowsWindow>(descriptor);
    }

    void Platform::showCursor(bool show) {
       
    }
}