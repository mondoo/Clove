#include "LinuxApplication.hpp"

#include "Platform/Linux/LinuxWindow.hpp"

namespace clv::plt{
	std::unique_ptr<Window> LinuxApplication::createWindow(const WindowProps& props){
		return std::make_unique<LinuxWindow>(props);
	}

	std::unique_ptr<Window> LinuxApplication::createWindow(const WindowProps& props, gfx::API api){
		return std::make_unique<LinuxWindow>(props, api);
	}
}