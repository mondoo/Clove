#include "Clove/Platform/Linux/LinuxApplication.hpp"

#include "Clove/Platform/Linux/LinuxWindow.hpp"

namespace clv::plt{
	gfx::API LinuxApplication::getPlatformPreferedAPI(){
		return gfx::API::OpenGL4;
	}

	std::unique_ptr<Window> LinuxApplication::createWindow(const WindowProps& props){
		return std::make_unique<LinuxWindow>(props);
	}

    std::unique_ptr<Window> LinuxApplication::createChildWindow(const Window& parentWindow, const mth::vec2i& position, const mth::vec2i& size){
        return std::make_unique<LinuxWindow>(parentWindow, position, size);
	}
}