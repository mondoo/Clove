#pragma once

namespace clv::gfx {
    enum class API {
        None,
#if GARLIC_PLATFORM_WINDOWS
        Vulkan,
#elif GARLIC_PLATFORM_LINUX
        Vulkan,
#elif GARLIC_PLATFORM_MACOS
//TODO: Metal
#endif
    };
}