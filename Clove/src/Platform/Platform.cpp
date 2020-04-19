#include "Clove/Platform/Platform.hpp"

#if CLV_PLATFORM_WINDOWS
	#include "Clove/Platform/Windows/WindowsPlatform.hpp"
#elif CLV_PLATFORM_LINUX
	#include "Clove/Platform/Linux/LinuxPlatform.hpp"
#elif CLV_PLATFORM_MACOS
	#include "Clove/Platform/Mac/MacPlatform.hpp"
#endif

namespace clv::plt{
	std::unique_ptr<Platform> createPlatformInstance(){
	#if CLV_PLATFORM_WINDOWS
		return std::make_unique<plt::WindowsPlatform>();
	#elif CLV_PLATFORM_LINUX
		return std::make_unique<plt::LinuxPlatform>();
	#elif CLV_PLATFORM_MACOS
		return std::make_unique<plt::MacPlatform>();
	#endif
	}
}
