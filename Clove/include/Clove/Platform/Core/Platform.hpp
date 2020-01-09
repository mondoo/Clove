#pragma once

#include "Clove/Platform/Core/PlatformTypes.hpp"
#include "Clove/Graphics/Core/GraphicsTypes.hpp"

namespace clv::gfx{
	class RenderFactory;
}

namespace clv::plt{
	class Window;
}

namespace clv::plt{
	class Platform{
		//FUNCTIONS
	public:
		virtual ~Platform() = default;

		virtual gfx::RenderFactory& getGraphicsFactory() = 0;

		virtual std::shared_ptr<Window> createWindow(const WindowProps& props) = 0;
		virtual std::shared_ptr<Window> createChildWindow(const Window& parentWindow, const mth::vec2i& position, const mth::vec2i& size) = 0;
	};
}

namespace clv::plt{
	inline std::unique_ptr<Platform> createPlatformInstance();
	inline std::unique_ptr<Platform> createPlatformInstance(clv::gfx::API api);

	inline clv::gfx::API getPlatformPreferedAPI();
}