#include "Clove/Platform/Linux/LinuxApplication.hpp"

#include "Clove/Platform/Linux/LinuxWindow.hpp"

namespace clv::plt{
	gfx::API LinuxApplication::getPlatformPreferedAPI(){
		return gfx::API::OpenGL4;
	}

	std::unique_ptr<Window> LinuxApplication::createWindow(const WindowProps& props){
		return std::make_unique<LinuxWindow>(props);
	}
}