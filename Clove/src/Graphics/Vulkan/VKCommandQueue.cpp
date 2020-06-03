#include "Clove/Graphics/Vulkan/VKCommandQueue.hpp"

namespace clv::gfx::vk {
	VKGraphicsQueue::VKGraphicsQueue(VkDevice device, uint32_t queueFamilyIndex, CommandQueueDescriptor descriptor)
		: queueFamilyIndex(queueFamilyIndex)
		, queue(queue) {

		//VkCommandPoolCreateInfo graphicsPoolInfo{};
		//graphicsPoolInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
		//graphicsPoolInfo.queueFamilyIndex = queueFamilyIndex;
		//graphicsPoolInfo.flags = 0;

		//if(vkCreateCommandPool(device, &graphicsPoolInfo, nullptr, &commandPool) != VK_SUCCESS) {
		//	//throw std::runtime_error("failed to create graphics command pool!");
		//}

	}

	VKGraphicsQueue::~VKGraphicsQueue() {

	}

	VKPresentQueue::VKPresentQueue(VkDevice device, uint32_t queueFamilyIndex) {
	}

	VKPresentQueue::~VKPresentQueue() {
	}

	VKTransferQueue::VKTransferQueue(VkDevice device, uint32_t queueFamilyIndex, CommandQueueDescriptor descriptor) {
	}

	VKTransferQueue::~VKTransferQueue() {
	}
}