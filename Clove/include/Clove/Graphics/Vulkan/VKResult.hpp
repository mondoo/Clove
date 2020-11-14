#pragma once

#include "Clove/Graphics/Result.hpp"

#include <vulkan/vulkan.h>

namespace clv::gfx::vk {
    Result convertResult(VkResult result);
}