#include "Clove/Platform/Windows/WindowsApplication.hpp"

#include "Clove/Platform/Windows/WindowsWindow.hpp"

namespace clv::plt{
	gfx::API WindowsApplication::getPlatformPreferedAPI(){
		return gfx::API::DirectX11;
	}

	std::unique_ptr<Window> WindowsApplication::createWindow(const WindowProps& props){
		return std::make_unique<WindowsWindow>(props);
	}

	std::unique_ptr<Window> WindowsApplication::createChildWindow(const Window& parentWindow, const mth::vec2i& position, const mth::vec2i& size){
		return std::make_unique<WindowsWindow>(parentWindow, position, size);
	}
}