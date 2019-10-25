#pragma once

#include "Clove/Platform/PlatformTypes.hpp"
#include "Clove/Graphics/GraphicsTypes.hpp"

namespace clv::plt{
	class Window;

	std::unique_ptr<Window> createWindow(const WindowProps& props = WindowProps());
	std::unique_ptr<Window> createWindow(const WindowProps& props, gfx::API api);
}