#pragma once

//TODO: Remove
#include <vulkan/vulkan.h>

namespace clv::gfx::vk {
	enum class QueueType {
		Graphics,
		Transfer,
		Present
	};

	enum class QueueFlags {
		None,
		Transient, //Buffers will be short lived
	};

	struct CommandQueueDescriptor{
		QueueType type;
		QueueFlags flags;
	};
}

namespace clv::gfx::vk {
	class VKCommandQueue {
		//VARIABLES
	private:
		uint32_t queueFamilyIndex = 0;
		VkQueue queue = VK_NULL_HANDLE;
		VkCommandPool commandPool = VK_NULL_HANDLE;

		//FUNCTIONS
	public:
		//TODO: Ctors
		VKCommandQueue() = delete;
		VKCommandQueue(uint32_t queueFamilyIndex, VkQueue queue, QueueFlags flags);
	};
}