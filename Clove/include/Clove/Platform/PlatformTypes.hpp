#pragma once

#include "Clove/Graphics/GraphicsTypes.hpp"

namespace clv::plt{
	constexpr clv::gfx::API getPlatformPreferedAPI(){
	#if GARLIC_PLATFORM_WINDOWS
		return clv::gfx::API::Direct3D11;
	#elif GARLIC_PLATFORM_LINUX
		return clv::gfx::API::OpenGL4;
	#elif GARLIC_PLATFORM_MACOS
		return clv::gfx::API::Metal1;
	#endif
	}

	struct WindowDescriptor{
		std::string title;
		int32_t width;
		int32_t height;
		gfx::API api = getPlatformPreferedAPI();
	};
}