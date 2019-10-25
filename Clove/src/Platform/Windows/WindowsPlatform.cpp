#include "Clove/Platform/Platform.hpp"

#include "Platform/Windows/WindowsWindow.hpp"

namespace clv::plt{
	std::unique_ptr<Window> createWindow(const WindowProps& props){
		return std::make_unique<WindowsWindow>(props);
	}

	std::unique_ptr<Window> createWindow(const WindowProps& props, gfx::API api){
		return std::make_unique<WindowsWindow>(props, api);
	}
}