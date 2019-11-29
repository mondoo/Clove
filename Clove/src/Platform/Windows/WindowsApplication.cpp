#include "Clove/Platform/Windows/WindowsApplication.hpp"

#include "Clove/Platform/Windows/WindowsWindow.hpp"

namespace clv::plt{
	std::unique_ptr<Window> WindowsApplication::createWindow(const WindowProps& props){
		return std::make_unique<WindowsWindow>(props);
	}

	gfx::API WindowsApplication::getPlatformPreferedAPI(){
		return gfx::API::DirectX11;
	}
}