#pragma once

#include "Clove/Graphics/CommandBuffer.hpp"

#include <vulkan/vulkan.h>

namespace garlic::clove {
    VkCommandBufferUsageFlags getCommandBufferUsageFlags(CommandBufferUsage garlicUsage);
}