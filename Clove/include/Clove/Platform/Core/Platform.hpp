#pragma once

#include "Clove/Platform/Core/PlatformTypes.hpp"
#include "Clove/Graphics/Core/GraphicsTypes.hpp"

namespace clv::gfx{
	class GraphicsFactory;
}

namespace clv::plt{
	class Window;
}

namespace clv::plt{
	class Platform{
		//FUNCTIONS
	public:
		virtual ~Platform() = default;

		virtual gfx::GraphicsFactory& getGraphicsFactory() = 0;

		virtual std::shared_ptr<Window> createWindow(const WindowDescriptor& props) = 0;
		virtual std::shared_ptr<Window> createChildWindow(const Window& parentWindow, const mth::vec2i& position, const mth::vec2i& size) = 0;
	};
}

namespace clv::plt{
	std::unique_ptr<Platform> createPlatformInstance();
	std::unique_ptr<Platform> createPlatformInstance(clv::gfx::API api);
}