#pragma once

#include "Clove/Graphics/GraphicsResource.hpp"

#include <vulkan/vulkan.h>

namespace garlic::clove {
    VkAccessFlags convertAccessFlags(AccessFlags garlicAccess);
}