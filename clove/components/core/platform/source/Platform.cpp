#include "Clove/Platform/Platform.hpp"

#if CLOVE_PLATFORM_WINDOWS
    #include "Clove/Platform/Windows/WindowsPlatform.hpp"
#elif CLOVE_PLATFORM_LINUX
    #include "Clove/Platform/Linux/LinuxPlatform.hpp"
#elif CLOVE_PLATFORM_MACOS
    #include "Clove/Platform/Mac/MacPlatform.hpp"
#endif

namespace garlic::clove {
    std::unique_ptr<Platform> createPlatformInstance() {
#if CLOVE_PLATFORM_WINDOWS
        return std::make_unique<WindowsPlatform>();
#elif CLOVE_PLATFORM_LINUX
        return std::make_unique<LinuxPlatform>();
#elif CLOVE_PLATFORM_MACOS
        return std::make_unique<MacPlatform>();
#endif
    }
}
