#include "WindowsApplication.hpp"

#include "Platform/Windows/WindowsWindow.hpp"

namespace clv::plt{
	std::unique_ptr<Window> WindowsApplication::createWindow(const WindowProps& props){
		return std::make_unique<WindowsWindow>(props);
	}
}