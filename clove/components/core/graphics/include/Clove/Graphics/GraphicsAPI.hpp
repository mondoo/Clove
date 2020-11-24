#pragma once

namespace garlic::clove {
    enum class GraphicsApi {
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