#pragma once

#include "Clove/Graphics/CommandBuffer.hpp"

#include <vulkan/vulkan.h>

namespace clv::gfx::vk {
    VkCommandBufferUsageFlags getCommandBufferUsageFlags(CommandBufferUsage garlicUsage);
}