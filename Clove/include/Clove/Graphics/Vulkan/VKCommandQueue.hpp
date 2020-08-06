#pragma once

//TODO: Remove
#include <vulkan/vulkan.h>
#include "Clove/Graphics/Vulkan/VKTransferCommandBuffer.hpp"
#include "Clove/Graphics/Vulkan/VKGraphicsCommandBuffer.hpp"

#include "Clove/Graphics/Vulkan/VKSemaphore.hpp"
#include "Clove/Graphics/Vulkan/VKFence.hpp"

namespace clv::gfx {
	enum class QueueFlags {
		None,
		Transient, //Buffers will be short lived
	};

	struct CommandQueueDescriptor{
		QueueFlags flags;
	};

	//Graphic ssubmit info:
	enum class WaitStage{
		ColourAttachmentOutput
	};

	struct GraphicsSubmitInfo {
		//Each element in the semaphore maps to an element in the waitStages
		std::vector<std::shared_ptr<vk::VKSemaphore>> waitSemaphores;
		std::vector<WaitStage> waitStages;

		std::vector<std::shared_ptr<vk::VKGraphicsCommandBuffer>> commandBuffers;

		std::vector<std::shared_ptr<vk::VKSemaphore>> signalSemaphores;
	};

	struct PresentInfo{
		std::vector<std::shared_ptr<vk::VKSemaphore>> waitSemaphores;

		std::shared_ptr<clv::gfx::vk::VKSwapchain> swapChain;
		uint32_t imageIndex;
	};

	struct TransferSubmitInfo{
        std::vector<std::shared_ptr<vk::VKTransferCommandBuffer>> commandBuffers;
	};
}

namespace clv::gfx::vk {
	//TODO: Move out into different files

	//Creates buffers for encoding graphics commands
	class VKGraphicsQueue {
		//VARIABLES
	private:
		//uint32_t queueFamilyIndex = 0;
		VkDevice device = VK_NULL_HANDLE;
		VkQueue queue = VK_NULL_HANDLE;
		VkCommandPool commandPool = VK_NULL_HANDLE;

		//FUNCTIONS
	public:
		//TODO: Ctors
		VKGraphicsQueue() = delete;
		VKGraphicsQueue(VkDevice device, uint32_t queueFamilyIndex, CommandQueueDescriptor descriptor);

		~VKGraphicsQueue();

		std::unique_ptr<VKGraphicsCommandBuffer> allocateCommandBuffer();
        void freeCommandBuffer(VKGraphicsCommandBuffer& buffer);

		void submit(const GraphicsSubmitInfo& submitInfo, const std::shared_ptr<VKFence>& fence);
	};

	//Allows for presentation of swapchains
	class VKPresentQueue {
		//VARIABLES
	private:
		//uint32_t queueFamilyIndex = 0;
		VkDevice device = VK_NULL_HANDLE;
		VkQueue queue = VK_NULL_HANDLE;

		//FUNCTIONS
	public:
		//TODO: Ctors
		VKPresentQueue() = delete;
		VKPresentQueue(VkDevice device, uint32_t queueFamilyIndex);

		~VKPresentQueue();

		Result present(const PresentInfo& presentInfo);
	};

	//Creates buffers for encoding transfer commands
	class VKTransferQueue {
		//VARIABLES
	private:
		//uint32_t queueFamilyIndex = 0;
		VkDevice device = VK_NULL_HANDLE;
		VkQueue queue = VK_NULL_HANDLE;
		VkCommandPool commandPool = VK_NULL_HANDLE;

		//FUNCTIONS
	public:
		//TODO: Ctors
		VKTransferQueue() = delete;
		VKTransferQueue(VkDevice device, uint32_t queueFamilyIndex, CommandQueueDescriptor descriptor);

		~VKTransferQueue();

		std::unique_ptr<VKTransferCommandBuffer> allocateCommandBuffer();
        void freeCommandBuffer(VKTransferCommandBuffer& buffer);

		void submit(const TransferSubmitInfo& submitInfo);
	};
}