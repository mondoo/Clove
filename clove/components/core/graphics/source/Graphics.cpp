#include "Clove/Graphics/Graphics.hpp"

#if CLOVE_PLATFORM_WINDOWS
    #include "Clove/Graphics/Vulkan/VulkanDevice.hpp"
#elif CLOVE_PLATFORM_MACOS
    #include "Clove/Graphics/Metal/MetalDevice.hpp"
#elif CLOVE_PLATFORM_LINUX
    #include "Clove/Graphics/Vulkan/VulkanDevice.hpp"
#endif

#if CLOVE_GHA_VALIDATION
    #include "Clove/Graphics/Validation/ValidationLog.hpp"
#endif

#include <Clove/Definitions.hpp>
#include <Clove/Log/Log.hpp>

CLOVE_DECLARE_LOG_CATEGORY(CloveGha)

namespace clove {
    std::unique_ptr<GhaDevice> createGraphicsDevice(GraphicsApi api, std::any nativeWindow) {
#if CLOVE_GHA_VALIDATION
        CLOVE_LOG(CloveGhaValidation, LogLevel::Debug, "GHA validation enabled.");
#endif

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
                CLOVE_ASSERT_MSG(false, "{0}: Default statement hit. Could not create GHA device.", CLOVE_FUNCTION_NAME);
                break;
        }

        if(device != nullptr) {
            GhaDevice::Info const info{ device->getInfo() };

            CLOVE_LOG(CloveGha, LogLevel::Trace, "GHA device created with a {0} backend.", info.ApiName);
            if(info.ApiVersion.has_value()) {
                CLOVE_LOG(CloveGha, LogLevel::Trace, "\tAPI:\t{0}.{1}.{2}", info.ApiVersion->major, info.ApiVersion->minor, info.ApiVersion->patch);
            }
            CLOVE_LOG(CloveGha, LogLevel::Trace, "\tDevice:\t{0}", info.deviceName);
            if(info.driverVersion.has_value()) {
                CLOVE_LOG(CloveGha, LogLevel::Trace, "\tDriver:\t{0}.{1}.{2}", info.driverVersion->major, info.driverVersion->minor, info.driverVersion->patch);
            }
        } else {
            CLOVE_LOG(CloveGha, LogLevel::Error, "GHA Device creation failed.");
        }

        return device;
    }
}
