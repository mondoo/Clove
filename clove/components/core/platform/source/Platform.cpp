#include "Clove/Platform/Platform.hpp"

#if GARLIC_PLATFORM_WINDOWS
    #include "Clove/Platform/Windows/WindowsPlatform.hpp"
#elif GARLIC_PLATFORM_LINUX
    #include "Clove/Platform/Linux/LinuxPlatform.hpp"
#elif GARLIC_PLATFORM_MACOS
    #include "Clove/Platform/Mac/MacPlatform.hpp"
#endif

namespace garlic::clove {
    std::unique_ptr<Platform> createPlatformInstance() {
#if GARLIC_PLATFORM_WINDOWS
        return std::make_unique<WindowsPlatform>();
#elif GARLIC_PLATFORM_LINUX
        return std::make_unique<LinuxPlatform>();
#elif GARLIC_PLATFORM_MACOS
        return std::make_unique<MacPlatform>();
#endif
    }
}
