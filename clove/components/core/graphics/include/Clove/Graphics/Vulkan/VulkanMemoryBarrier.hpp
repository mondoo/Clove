#pragma once

#include "Clove/Graphics/MemoryBarrier.hpp"
#include "Clove/Graphics/Vulkan/VulkanTypes.hpp"

namespace garlic::clove {
    uint32_t getQueueFamilyIndex(QueueType type, QueueFamilyIndices const &indices);
}