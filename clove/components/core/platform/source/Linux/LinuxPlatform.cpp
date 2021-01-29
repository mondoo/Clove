#include "Clove/Platform/Platform.hpp"

#include "Clove/Platform/Linux/LinuxWindow.hpp"

namespace garlic::clove {
    std::unique_ptr<Window> Platform::createWindow(WindowDescriptor const &descriptor) {
        return std::make_unique<LinuxWindow>(descriptor);
    }

    void Platform::showCursor(bool show) {
        
    }
}
