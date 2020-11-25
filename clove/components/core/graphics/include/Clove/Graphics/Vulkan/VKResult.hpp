#pragma once

#include "Clove/Graphics/Result.hpp"

#include <vulkan/vulkan.h>

namespace garlic::clove {
    Result convertResult(VkResult result);
}