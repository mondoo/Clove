#include "Clove/Platform/Windows/WindowsPlatform.hpp"

#include "Clove/Platform/Windows/WindowsWindow.hpp"

namespace clv::plt{
	WindowsPlatform::WindowsPlatform() = default;

	WindowsPlatform::WindowsPlatform(WindowsPlatform&& other) noexcept = default;

	WindowsPlatform& WindowsPlatform::operator=(WindowsPlatform&& other) noexcept = default;

	WindowsPlatform::~WindowsPlatform() = default;

	std::shared_ptr<Window> WindowsPlatform::createWindow(const WindowDescriptor& descriptor){
		return std::make_shared<WindowsWindow>(descriptor);
	}

	std::shared_ptr<Window> WindowsPlatform::createChildWindow(const Window& parentWindow, const mth::vec2i& position, const mth::vec2i& size, const gfx::API api){
		return std::make_shared<WindowsWindow>(parentWindow, position, size, api);
	}
}