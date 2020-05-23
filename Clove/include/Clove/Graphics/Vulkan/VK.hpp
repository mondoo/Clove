#pragma once

#include <utility>
//TODO: Move into cpp
#include <vulkan/vulkan.h>

namespace clv::gfx {
	class GraphicsFactory;
}

namespace clv::gfx::vk {
	//TODO: return graphics factory
	std::pair<VkInstance, VkDebugUtilsMessengerEXT> initialiseVK();
}