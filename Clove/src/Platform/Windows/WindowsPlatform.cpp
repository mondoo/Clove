#include "Clove/Platform/Windows/WindowsPlatform.hpp"

#include "Clove/Platform/Windows/WindowsWindow.hpp"
#include "Clove/Graphics/Core/Graphics.hpp"
#include "Clove/Graphics/Core/RenderFactory.hpp"

namespace clv::plt{
	WindowsPlatform::WindowsPlatform(gfx::API api){
		graphicsFactory = gfx::initialise(api);
	}

	WindowsPlatform::WindowsPlatform(WindowsPlatform&& other) noexcept = default;

	WindowsPlatform& WindowsPlatform::operator=(WindowsPlatform&& other) noexcept = default;

	WindowsPlatform::~WindowsPlatform() = default;

	gfx::RenderFactory& WindowsPlatform::getGraphicsFactory(){
		return *graphicsFactory;
	}

	std::shared_ptr<Window> WindowsPlatform::createWindow(const WindowProps& props){
		return std::make_shared<WindowsWindow>(props);
	}

	std::shared_ptr<Window> WindowsPlatform::createChildWindow(const Window& parentWindow, const mth::vec2i& position, const mth::vec2i& size){
		return std::make_shared<WindowsWindow>(parentWindow, position, size);
	}
}