#include "Clove/Platform/Platform.hpp"

#include "Platform/Windows/WindowsWindow.hpp"

namespace clv::plt{
	void Platform::prepare(){
		//Empty
	}

	std::unique_ptr<Window> Platform::createWindow(const WindowProps& props){
		return std::make_unique<WindowsWindow>(props);
	}

	std::unique_ptr<Window> Platform::createWindow(const WindowProps& props, gfx::API api){
		return std::make_unique<WindowsWindow>(props, api);
	}
}