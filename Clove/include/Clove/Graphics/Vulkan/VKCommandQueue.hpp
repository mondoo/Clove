#pragma once

//TODO: Remove
#include <vulkan/vulkan.h>
#include "Clove/Graphics/Vulkan/VKCommandBuffer.hpp"

#include "Clove/Graphics/Vulkan/VKSemaphore.hpp"

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

		std::vector<std::shared_ptr<vk::VKCommandBuffer>> commandBuffers;

		std::vector<std::shared_ptr<vk::VKSemaphore>> signalSemaphores;
	};

	struct PresentInfo{
		std::vector<std::shared_ptr<vk::VKSemaphore>> waitSemaphores;

		std::shared_ptr<clv::gfx::vk::VKSwapchain> swapChain;
		uint32_t imageIndex;
	};
}

namespace clv::gfx::vk {
	//TODO: Move out into different files
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

		std::unique_ptr<VKCommandBuffer> allocateCommandBuffer();

		void submit(const GraphicsSubmitInfo& submitInfo);
	};

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

		void present(const PresentInfo& presentInfo);
	};

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

		std::unique_ptr<VKCommandBuffer> allocateCommandBuffer();
	};
}