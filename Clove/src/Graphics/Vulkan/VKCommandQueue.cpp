#include "Clove/Graphics/Vulkan/VKCommandQueue.hpp"

namespace clv::gfx::vk {
	static VkPipelineStageFlagBits getPipelineStageFlag(WaitStage stage) {
		switch(stage) {
			case WaitStage::ColourAttachmentOutput:
				return VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
		}
	}

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

	void VKGraphicsQueue::submit(const GraphicsSubmitInfo& submitInfo) {
		//Wait semaphores / stages
		const size_t waitSemaphoreCount = std::size(submitInfo.waitSemaphores);
		std::vector<VkSemaphore> waitSemaphores(waitSemaphoreCount);
		for(size_t i = 0; i < waitSemaphoreCount; ++i) {
			waitSemaphores[i] = submitInfo.waitSemaphores[i]->getSemaphore();
		}
		std::vector<VkPipelineStageFlags> waitStages(waitSemaphoreCount);
		for(size_t i = 0; i < waitSemaphoreCount; i++) {
			waitStages[i] = getPipelineStageFlag(submitInfo.waitStages[i]);
		}

		//Command buffers
		const size_t commandBufferCount = std::size(submitInfo.commandBuffers);
		std::vector<VkCommandBuffer> commandBuffers(commandBufferCount);
		for(size_t i = 0; i < commandBufferCount; i++) {
			commandBuffers[i] = submitInfo.commandBuffers[i]->getCommandBuffer();
		}

		//Signal semaphores
		const size_t signalSemaphoreCount = std::size(submitInfo.signalSemaphores);
		std::vector<VkSemaphore> signalSemaphores(signalSemaphoreCount);
		for(size_t i = 0; i < signalSemaphoreCount; i++) {
			signalSemaphores[i] = submitInfo.signalSemaphores[i]->getSemaphore();
		}

		VkSubmitInfo vkSubmitInfo{};
		vkSubmitInfo.sType				  = VK_STRUCTURE_TYPE_SUBMIT_INFO;
		vkSubmitInfo.waitSemaphoreCount	  = waitSemaphoreCount;
		vkSubmitInfo.pWaitSemaphores	  = std::data(waitSemaphores);
		vkSubmitInfo.pWaitDstStageMask	  = std::data(waitStages);
		vkSubmitInfo.commandBufferCount	  = commandBufferCount;
		vkSubmitInfo.pCommandBuffers	  = std::data(commandBuffers);
		vkSubmitInfo.signalSemaphoreCount = signalSemaphoreCount;
		vkSubmitInfo.pSignalSemaphores	  = std::data(signalSemaphores);

		if(vkQueueSubmit(queue, 1, &vkSubmitInfo, VK_NULL_HANDLE) != VK_SUCCESS) {
			GARLIC_LOG(garlicLogContext, Log::Level::Error, "Failed to submit graphics command buffer(s)");
		}
	}

	VKPresentQueue::VKPresentQueue(VkDevice device, uint32_t queueFamilyIndex) {
		vkGetDeviceQueue(device, queueFamilyIndex, 0, &queue);
	}

	VKPresentQueue::~VKPresentQueue() = default;

	void VKPresentQueue::present(const PresentInfo& presentInfo) {
		//Wait semaphores
		const size_t waitSemaphoreCount = std::size(presentInfo.waitSemaphores);
		std::vector<VkSemaphore> waitSemaphores(waitSemaphoreCount);
		for(size_t i = 0; i < waitSemaphoreCount; ++i) {
			waitSemaphores[i] = presentInfo.waitSemaphores[i]->getSemaphore();
		}

		VkPresentInfoKHR vkpresentInfo{};
		vkpresentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
		vkpresentInfo.waitSemaphoreCount = waitSemaphoreCount;
		vkpresentInfo.pWaitSemaphores	 = std::data(waitSemaphores);
		vkpresentInfo.swapchainCount	 = 1;
		vkpresentInfo.pSwapchains		 = &presentInfo.swapChain->getSwapchain();
		vkpresentInfo.pImageIndices		 = &presentInfo.imageIndex;

		//TODO: Error handling
		vkQueuePresentKHR(queue, &vkpresentInfo);
	}

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