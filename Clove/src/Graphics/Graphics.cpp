#include "Clove/Graphics/Graphics.hpp"

#if GARLIC_PLATFORM_WINDOWS
	#include "Clove/Graphics/Vulkan/VKGraphicsDevice.hpp"
#elif GARLIC_PLATFORM_LINUX
	#include "Clove/Graphics/Vulkan/VKGraphicsDevice.hpp"
#elif GARLIC_PLATFORM_MACOS
	//TODO: Metal
#endif
#include "Clove/Log.hpp"

#include <Root/Definitions.hpp>

namespace clv::gfx{
    std::unique_ptr<GraphicsDevice> createGraphicsDevice(API api, void* nativeWindow) {
		switch(api){
			#if GARLIC_PLATFORM_WINDOWS
			case API::Vulkan:
                return std::make_unique<vk::VKGraphicsDevice>(nativeWindow);

			#elif GARLIC_PLATFORM_LINUX
            case API::Vulkan:
                return std::make_unique<vk::VKGraphicsDevice>(nativeWindow);
				
			#elif GARLIC_PLATFORM_MACOS
			//TODO: Metal
			#endif

			default:
				GARLIC_LOG(garlicLogContext, Log::Level::Error, "Default statement hit. Could not initialise RenderAPI: {0}", GARLIC_FUNCTION_NAME);
                return nullptr;
		}
	}
}
