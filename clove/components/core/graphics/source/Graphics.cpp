#include "Clove/Graphics/Graphics.hpp"

#if GARLIC_PLATFORM_WINDOWS
    #include "Clove/Graphics/Vulkan/VKGraphicsDevice.hpp"
#elif GARLIC_PLATFORM_MACOS
#elif GARLIC_PLATFORM_LINUX
    #include "Clove/Graphics/Vulkan/VKGraphicsDevice.hpp"
#endif

#include <Clove/Definitions.hpp>
#include <Clove/Log/Log.hpp>

namespace garlic::clove {
    std::unique_ptr<GraphicsDevice> createGraphicsDevice(GraphicsApi api, std::any nativeWindow) {
        switch(api) {
#if GARLIC_PLATFORM_WINDOWS
            case GraphicsApi::Vulkan:
                return std::make_unique<VKGraphicsDevice>(std::move(nativeWindow));
#elif GARLIC_PLATFORM_MACOS
#elif GARLIC_PLATFORM_LINUX
            case GraphicsApi::Vulkan:
                return std::make_unique<VKGraphicsDevice>(std::move(nativeWindow));
#endif
            default:
                GARLIC_LOG(garlicLogContext, LogLevel::Error, "Default statement hit. Could not initialise RenderAPI: {0}", GARLIC_FUNCTION_NAME);
                return nullptr;
        }
    }
}
