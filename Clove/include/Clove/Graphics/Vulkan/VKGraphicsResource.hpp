#pragma once

#include "Clove/Graphics/GraphicsResource.hpp"

#include <vulkan/vulkan.h>

namespace clv::gfx::vk {
    VkAccessFlags convertAccessFlags(AccessFlags garlicAccess);
}