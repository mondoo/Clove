#pragma once

#include "Clove/Platform/PlatformTypes.hpp"
#include "Clove/Graphics/GraphicsTypes.hpp"

namespace clv::plt{
	class Window;

	class Platform{
		//FUNCTIONS
	public:
		static void prepare();
		
		static std::unique_ptr<Window> createWindow(const WindowProps& props = WindowProps());
		static std::unique_ptr<Window> createWindow(const WindowProps& props, gfx::API api);
	};
}
