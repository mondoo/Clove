#include "Core/Platform/Platform.hpp"

#include "Platform/Linux/LinuxWindow.hpp"

namespace clv::plt{
	void Platform::prepare(){
		//Empty
	}
	
	std::unique_ptr<Window> Platform::createWindow(const WindowProps& props){
		return std::make_unique<LinuxWindow>(props);
	}

	std::unique_ptr<Window> Platform::createWindow(const WindowProps& props, gfx::API api){
		return std::make_unique<LinuxWindow>(props, api);
	}
}