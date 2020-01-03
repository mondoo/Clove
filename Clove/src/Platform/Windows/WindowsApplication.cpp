#include "Clove/Platform/Windows/WindowsApplication.hpp"

#include "Clove/Platform/Windows/WindowsWindow.hpp"

namespace clv::plt{
	WindowsApplication::WindowsApplication(gfx::API api)
		: Application(api){
	}

	std::shared_ptr<Window> WindowsApplication::createWindow(const WindowProps& props){
		return std::make_shared<WindowsWindow>(props);
	}

	std::shared_ptr<Window> WindowsApplication::createChildWindow(const Window& parentWindow, const mth::vec2i& position, const mth::vec2i& size){
		return std::make_shared<WindowsWindow>(parentWindow, position, size);
	}
}