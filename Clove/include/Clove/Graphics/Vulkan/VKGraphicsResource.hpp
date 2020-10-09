#pragma once

#include "Clove/Graphics/GraphicsResource.hpp"

#include <vulkan/vulkan.h>

namespace clv::gfx::vk {
    VkMemoryPropertyFlags getMemoryPropertyFlags(MemoryType garlicProperties);

    VkAccessFlags convertAccessFlags(AccessFlags garlicAccess);
}