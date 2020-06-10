#include "Clove/Graphics/Vulkan/VKCommandQueue.hpp"

namespace clv::gfx::vk {
	VKGraphicsQueue::VKGraphicsQueue(VkDevice device, uint32_t queueFamilyIndex, CommandQueueDescriptor descriptor)
		: device(device) {
		vkGetDeviceQueue(device, queueFamilyIndex, 0, &queue);

		VkCommandPoolCreateInfo poolInfo{};
		poolInfo.sType			  = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
		poolInfo.queueFamilyIndex = queueFamilyIndex;
		poolInfo.flags			  = descriptor.flags == QueueFlags::Transient ? VK_COMMAND_POOL_CREATE_TRANSIENT_BIT : 0;

		if(vkCreateCommandPool(device, &poolInfo, nullptr, &commandPool) != VK_SUCCESS) {
			GARLIC_LOG(garlicLogContext, Log::Level::Error, "Failed to create graphics command pool");
		}
	}

	VKGraphicsQueue::~VKGraphicsQueue() {
		vkDestroyCommandPool(device, commandPool, nullptr);
	}

	std::unique_ptr<VKCommandBuffer> VKGraphicsQueue::allocateCommandBuffer() {
		//TODO: Multiple command buffer allocation

		VkCommandBuffer commandBuffer;

		VkCommandBufferAllocateInfo allocInfo{};
		allocInfo.sType				 = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
		allocInfo.commandPool		 = commandPool;
		allocInfo.level				 = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
		allocInfo.commandBufferCount = 1;

		if(vkAllocateCommandBuffers(device, &allocInfo, &commandBuffer) != VK_SUCCESS) {
			GARLIC_LOG(garlicLogContext, Log::Level::Error, "Failed to allocate command buffer");
			return nullptr;
		}

		return std::make_unique<VKCommandBuffer>(commandBuffer);
	}

	VKPresentQueue::VKPresentQueue(VkDevice device, uint32_t queueFamilyIndex) {
		vkGetDeviceQueue(device, queueFamilyIndex, 0, &queue);
	}

	VKPresentQueue::~VKPresentQueue() = default;

	VKTransferQueue::VKTransferQueue(VkDevice device, uint32_t queueFamilyIndex, CommandQueueDescriptor descriptor)
		: device(device) {
		vkGetDeviceQueue(device, queueFamilyIndex, 0, &queue);

		VkCommandPoolCreateInfo poolInfo{};
		poolInfo.sType			  = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
		poolInfo.queueFamilyIndex = queueFamilyIndex;
		poolInfo.flags			  = descriptor.flags == QueueFlags::Transient ? VK_COMMAND_POOL_CREATE_TRANSIENT_BIT : 0;

		if(vkCreateCommandPool(device, &poolInfo, nullptr, &commandPool) != VK_SUCCESS) {
			GARLIC_LOG(garlicLogContext, Log::Level::Error, "Failed to create graphics command pool");
		}
	}

	VKTransferQueue::~VKTransferQueue() {
		vkDestroyCommandPool(device, commandPool, nullptr);
	}

	std::unique_ptr<VKCommandBuffer> VKTransferQueue::allocateCommandBuffer() {
		//TODO: Multiple command buffer allocation

		VkCommandBuffer commandBuffer;

		VkCommandBufferAllocateInfo allocInfo{};
		allocInfo.sType				 = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
		allocInfo.commandPool		 = commandPool;
		allocInfo.level				 = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
		allocInfo.commandBufferCount = 1;

		if(vkAllocateCommandBuffers(device, &allocInfo, &commandBuffer) != VK_SUCCESS) {
			GARLIC_LOG(garlicLogContext, Log::Level::Error, "Failed to allocate command buffer");
			return nullptr;
		}

		return std::make_unique<VKCommandBuffer>(commandBuffer);
	}
}