#pragma once

#include "Clove/Core/Platform/PlatformTypes.hpp"
#include "Clove/Core/Graphics/GraphicsTypes.hpp"

namespace clv::plt{
	class Window;
}

namespace clv::plt{
	class Platform{
		//FUNCTIONS
	public:
		virtual ~Platform() = default;

		virtual std::shared_ptr<Window> createWindow(const WindowProps& props) = 0;
		virtual std::shared_ptr<Window> createChildWindow(const Window& parentWindow, const mth::vec2i& position, const mth::vec2i& size) = 0;
	
		static std::unique_ptr<Platform> createPlatformInstance();
	};
}