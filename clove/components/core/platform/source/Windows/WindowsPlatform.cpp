#include "Clove/Platform/Platform.hpp"

#include "Clove/Platform/Windows/WindowsWindow.hpp"

namespace garlic::clove {
	void Platform::initialise() {}
	
    std::unique_ptr<Window> Platform::createWindow(WindowDescriptor const &descriptor) {
        return std::make_unique<WindowsWindow>(descriptor);
    }
}
