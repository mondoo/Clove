#include "LinuxApplication.hpp"

#include "Platform/Linux/LinuxWindow.hpp"

namespace clv::plt{
	std::unique_ptr<Window> LinuxApplication::createWindow(const WindowProps& props){
		return std::make_unique<LinuxWindow>(props);
	}

	gfx::API LinuxApplication::getPlatformPreferedAPI(){
		return gfx::API::OpenGL4;
	}
}