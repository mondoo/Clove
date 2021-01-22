#pragma once

#include "Clove/Graphics/Resource.hpp"

#include <vulkan/vulkan.h>

namespace garlic::clove {
    VkAccessFlags convertAccessFlags(AccessFlags garlicAccess);
}