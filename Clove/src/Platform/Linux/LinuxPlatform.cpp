#include "Clove/Platform/Linux/LinuxPlatform.hpp"

#include "Clove/Platform/Linux/LinuxWindow.hpp"

namespace clv::plt{
	LinuxPlatform::LinuxPlatform() = default;

	LinuxPlatform::LinuxPlatform(LinuxPlatform&& other) noexcept = default;

	LinuxPlatform& LinuxPlatform::operator=(LinuxPlatform&& other) noexcept = default;

	LinuxPlatform::~LinuxPlatform() = default;

	std::shared_ptr<Window> LinuxPlatform::createWindow(const WindowDescriptor& props){
		return std::make_shared<LinuxWindow>(props);
	}

    std::shared_ptr<Window> LinuxPlatform::createChildWindow(const Window& parentWindow, const mth::vec2i& position, const mth::vec2i& size, const gfx::API api){
        return std::make_shared<LinuxWindow>(parentWindow, position, size, api);
	}
}