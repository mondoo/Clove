#pragma once

//TODO: Remove
#include <vulkan/vulkan.h>

namespace clv::gfx::vk {
	enum class QueueFlags {
		None,
		Transient, //Buffers will be short lived
	};

	struct CommandQueueDescriptor{
		QueueFlags flags;
	};
}

namespace clv::gfx::vk {
	//TODO: Move out into different files
	class VKGraphicsQueue {
		//VARIABLES
	private:
		uint32_t queueFamilyIndex = 0;
		VkQueue queue = VK_NULL_HANDLE;
		VkCommandPool commandPool = VK_NULL_HANDLE;

		//FUNCTIONS
	public:
		//TODO: Ctors
		VKGraphicsQueue() = delete;
		VKGraphicsQueue(VkDevice device, uint32_t queueFamilyIndex, CommandQueueDescriptor descriptor);

		~VKGraphicsQueue();
	};

	class VKPresentQueue {
		//VARIABLES
	private:
		uint32_t queueFamilyIndex = 0;
		VkQueue queue = VK_NULL_HANDLE;

		//FUNCTIONS
	public:
		//TODO: Ctors
		VKPresentQueue() = delete;
		VKPresentQueue(VkDevice device, uint32_t queueFamilyIndex);

		~VKPresentQueue();
	};

	class VKTransferQueue {
		//VARIABLES
	private:
		uint32_t queueFamilyIndex = 0;
		VkQueue queue = VK_NULL_HANDLE;
		VkCommandPool commandPool = VK_NULL_HANDLE;

		//FUNCTIONS
	public:
		//TODO: Ctors
		VKTransferQueue() = delete;
		VKTransferQueue(VkDevice device, uint32_t queueFamilyIndex, CommandQueueDescriptor descriptor);

		~VKTransferQueue();
	};
}