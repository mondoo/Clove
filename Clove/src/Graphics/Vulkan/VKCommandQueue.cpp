#include "Clove/Graphics/Vulkan/VKCommandQueue.hpp"

namespace clv::gfx::vk {
	VKCommandQueue::VKCommandQueue(uint32_t queueFamilyIndex, VkQueue queue, QueueFlags flags)
		: queueFamilyIndex(queueFamilyIndex)
		, queue(queue){

	}
}