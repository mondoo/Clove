#include "Clove/Graphics/Graphics.hpp"

#if CLOVE_PLATFORM_WINDOWS
    #include "Clove/Graphics/Vulkan/VulkanDevice.hpp"
#elif CLOVE_PLATFORM_MACOS
    #include "Clove/Graphics/Metal/MetalDevice.hpp"
#elif CLOVE_PLATFORM_LINUX
    #include "Clove/Graphics/Vulkan/VulkanDevice.hpp"
#endif

#if CLOVE_GHA_VALIDATION
    #include "Clove/Graphics/Validation/ValidationDevice.hpp"
#endif

#include <Clove/Definitions.hpp>
#include <Clove/Log/Log.hpp>

namespace clove {
    std::unique_ptr<GhaDevice> createGraphicsDevice(GraphicsApi api, std::any nativeWindow) {
        std::unique_ptr<GhaDevice> device{ nullptr };

        switch(api) {
#if CLOVE_PLATFORM_WINDOWS
            case GraphicsApi::Vulkan:
                device = std::make_unique<VulkanDevice>(std::move(nativeWindow));
                break;
#elif CLOVE_PLATFORM_MACOS
            case GraphicsApi::Metal:
                device = std::make_unique<MetalDevice>(std::move(nativeWindow));
                break;
#elif CLOVE_PLATFORM_LINUX
            case GraphicsApi::Vulkan:
                device = std::make_unique<VulkanDevice>(std::move(nativeWindow));
                break;
#endif
            default:
                CLOVE_ASSERT("Default statement hit. Could not initialise RenderAPI: {0}", CLOVE_FUNCTION_NAME);
                break;
        }

#if !CLOVE_GHA_VALIDATION
        return device;
#else
        return std::make_unique<ValidationDevice>(std::move(device));
#endif
    }
}
