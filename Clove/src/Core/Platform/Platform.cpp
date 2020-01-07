#include "Clove/Core/Platform/Platform.hpp"

#if CLV_PLATFORM_WINDOWS
	#include "Clove/Platform/Windows/WindowsPlatform.hpp"
#elif CLV_PLATFORM_LINUX
	#include "Clove/Platform/Linux/LinuxPlatform.hpp"
#elif CLV_PLATFORM_MACOS
	#include "Clove/Platform/Mac/MacPlatform.hpp"
#endif

namespace clv::plt{
	std::unique_ptr<Platform> Platform::createPlatformInstance(){
	#if CLV_PLATFORM_WINDOWS
		return std::make_unique<plt::WindowsPlatform>();
	#elif CLV_PLATFORM_LINUX
		return std::make_unique<plt::LinuxPlatform>();
	#elif CLV_PLATFORM_MACOS
		return std::make_unique<plt::MacPlatform>();
	#endif
	}

	clv::gfx::API Platform::getPlatformPreferedAPI(){
	#if CLV_PLATFORM_WINDOWS
		return clv::gfx::API::Direct3D11;
	#elif CLV_PLATFORM_LINUX
		return clv::gfx::API::OpenGL4;
	#elif CLV_PLATFORM_MACOS
		return clv::gfx::API::Metal1;
	#endif
	}
}