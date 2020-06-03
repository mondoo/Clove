#include "Clove/Graphics/Vulkan/VKCommandQueue.hpp"

namespace clv::gfx::vk {
	VKGraphicsQueue::VKGraphicsQueue(VkDevice device, uint32_t queueFamilyIndex, CommandQueueDescriptor descriptor) 
		: device(device) {
		vkGetDeviceQueue(device, queueFamilyIndex, 0, &queue);

		VkCommandPoolCreateInfo poolInfo{};
		poolInfo.sType				= VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
		poolInfo.queueFamilyIndex	= queueFamilyIndex;
		poolInfo.flags				= descriptor.flags == QueueFlags::Transient ? VK_COMMAND_POOL_CREATE_TRANSIENT_BIT : 0;

		if(vkCreateCommandPool(device, &poolInfo, nullptr, &commandPool) != VK_SUCCESS) {
			CLV_LOG_ERROR("Failed to create graphics command pool");
		}
	}

	VKGraphicsQueue::~VKGraphicsQueue() {
		vkDestroyCommandPool(device, commandPool, nullptr);
	}

	VKPresentQueue::VKPresentQueue(VkDevice device, uint32_t queueFamilyIndex) {
		vkGetDeviceQueue(device, queueFamilyIndex, 0, &queue);
	}

	VKPresentQueue::~VKPresentQueue() = default;

	VKTransferQueue::VKTransferQueue(VkDevice device, uint32_t queueFamilyIndex, CommandQueueDescriptor descriptor)
		: device(device) {
		vkGetDeviceQueue(device, queueFamilyIndex, 0, &queue);

		VkCommandPoolCreateInfo poolInfo{};
		poolInfo.sType				= VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
		poolInfo.queueFamilyIndex	= queueFamilyIndex;
		poolInfo.flags				= descriptor.flags == QueueFlags::Transient ? VK_COMMAND_POOL_CREATE_TRANSIENT_BIT : 0;

		if(vkCreateCommandPool(device, &poolInfo, nullptr, &commandPool) != VK_SUCCESS) {
			CLV_LOG_ERROR("Failed to create graphics command pool");
		}
	}

	VKTransferQueue::~VKTransferQueue() {
		vkDestroyCommandPool(device, commandPool, nullptr);
	}
}