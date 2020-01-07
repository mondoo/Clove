#include "Clove/Platform/Linux/LinuxPlatform.hpp"

#include "Clove/Platform/Linux/LinuxWindow.hpp"

namespace clv::plt{
	std::shared_ptr<Window> LinuxPlatform::createWindow(const WindowProps& props){
		return std::make_shared<LinuxWindow>(props);
	}

    std::shared_ptr<Window> LinuxPlatform::createChildWindow(const Window& parentWindow, const mth::vec2i& position, const mth::vec2i& size){
        return std::make_shared<LinuxWindow>(parentWindow, position, size);
	}
}