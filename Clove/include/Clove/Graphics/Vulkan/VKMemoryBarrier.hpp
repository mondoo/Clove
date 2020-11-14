#pragma once

#include "Clove/Graphics/MemoryBarrier.hpp"
#include "Clove/Graphics/Vulkan/VulkanTypes.hpp"

namespace clv::gfx::vk {
    uint32_t getQueueFamilyIndex(QueueType type, const QueueFamilyIndices& indices);
}