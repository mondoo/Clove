#include "Clove/Graphics/Vulkan/VKCommandBuffer.hpp"

namespace clv::gfx::vk {
	VKCommandBuffer::VKCommandBuffer(VkCommandBuffer commandBuffer)
		: commandBuffer(commandBuffer) {
	}
}