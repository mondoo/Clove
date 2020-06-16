#include "Clove/Graphics/Vulkan/VKFence.hpp"

namespace clv::gfx::vk {
	VKFence::VKFence(VkDevice device, FenceDescriptor desccriptor)
		: device(device) {
		VkFenceCreateInfo createInfo{};
		createInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
		createInfo.flags = desccriptor.createSignaled ? VK_FENCE_CREATE_SIGNALED_BIT : 0;

		vkCreateFence(device, &createInfo, nullptr, &fence);
	}

	VKFence::~VKFence() {
		vkDestroyFence(device, fence, nullptr);
	}

	void VKFence::waitForFence() {
		vkWaitForFences(device, 1, &fence, VK_TRUE, UINT64_MAX);
	}

	void VKFence::resetFence() {
		vkResetFences(device, 1, &fence);
	}
}