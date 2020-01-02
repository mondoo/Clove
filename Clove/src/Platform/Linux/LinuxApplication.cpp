#include "Clove/Platform/Linux/LinuxApplication.hpp"

#include "Clove/Platform/Linux/LinuxWindow.hpp"

namespace clv::plt{
	LinuxApplication::LinuxApplication(gfx::API api)
		: Application(api){
	}

	std::shared_ptr<Window> LinuxApplication::createWindow(const WindowProps& props){
		return std::make_shared<LinuxWindow>(props);
	}

    std::shared_ptr<Window> LinuxApplication::createChildWindow(const Window& parentWindow, const mth::vec2i& position, const mth::vec2i& size){
        return std::make_shared<LinuxWindow>(parentWindow, position, size);
	}
}