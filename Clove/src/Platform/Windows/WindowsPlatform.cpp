#include "Clove/Platform/Windows/WindowsPlatform.hpp"

#include "Clove/Platform/Windows/WindowsWindow.hpp"

namespace clv::plt{
	std::shared_ptr<Window> WindowsPlatform::createWindow(const WindowProps& props){
		return std::make_shared<WindowsWindow>(props);
	}

	std::shared_ptr<Window> WindowsPlatform::createChildWindow(const Window& parentWindow, const mth::vec2i& position, const mth::vec2i& size){
		return std::make_shared<WindowsWindow>(parentWindow, position, size);
	}
}