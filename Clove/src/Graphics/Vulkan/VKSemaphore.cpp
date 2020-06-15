#include "Clove/Graphics/Vulkan/VKSemaphore.hpp"

namespace clv::gfx::vk {
	VKSemaphore::VKSemaphore(VkDevice device) 
		: device(device){
		VkSemaphoreCreateInfo createInfo{};
		createInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;

		if(vkCreateSemaphore(device, &createInfo, nullptr, &semaphore) != VK_SUCCESS) {
			GARLIC_LOG(garlicLogContext, Log::Level::Error, "Failed to create semaphore");
		}
	}

	VKSemaphore::~VKSemaphore() {
		vkDestroySemaphore(device, semaphore, nullptr);
	}
}