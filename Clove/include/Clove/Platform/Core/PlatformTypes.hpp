#pragma once

#include "Clove/Graphics/Core/GraphicsTypes.hpp"

namespace clv::plt{
	constexpr clv::gfx::API getPlatformPreferedAPI(){
	#if CLV_PLATFORM_WINDOWS
		return clv::gfx::API::Direct3D11;
	#elif CLV_PLATFORM_LINUX
		return clv::gfx::API::OpenGL4;
	#elif CLV_PLATFORM_MACOS
		return clv::gfx::API::Metal1;
	#endif
	}

	struct WindowDescriptor{
		std::string title;
		int32 width;
		int32 height;
		gfx::API api = getPlatformPreferedAPI();
	};
}