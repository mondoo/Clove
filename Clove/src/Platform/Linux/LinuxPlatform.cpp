#include "Clove/Platform/Linux/LinuxPlatform.hpp"

#include "Clove/Platform/Linux/LinuxWindow.hpp"
#include "Clove/Graphics/Core/Graphics.hpp"
#include "Clove/Graphics/Core/RenderFactory.hpp"

namespace clv::plt{
	LinuxPlatform::LinuxPlatform(gfx::API api){
		graphicsFactory = gfx::initialise(api);
	}

	LinuxPlatform::LinuxPlatform(LinuxPlatform&& other) noexcept = default;

	LinuxPlatform& LinuxPlatform::operator=(LinuxPlatform&& other) noexcept = default;

	LinuxPlatform::~LinuxPlatform() = default;

	gfx::RenderFactory& LinuxPlatform::getGraphicsFactory(){
		return *graphicsFactory;
	}

	std::shared_ptr<Window> LinuxPlatform::createWindow(const WindowProps& props){
		return std::make_shared<LinuxWindow>(*graphicsFactory, props);
	}

    std::shared_ptr<Window> LinuxPlatform::createChildWindow(const Window& parentWindow, const mth::vec2i& position, const mth::vec2i& size){
        return std::make_shared<LinuxWindow>(*graphicsFactory, parentWindow, position, size);
	}
}