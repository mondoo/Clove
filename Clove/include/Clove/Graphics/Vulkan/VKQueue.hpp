#pragma once

#include "Clove/Graphics/Queue.hpp"

#include <vulkan/vk_icd.h>

namespace clv::gfx::vk {
    VkCommandPoolCreateFlags convertCommandPoolCreateFlags(QueueFlags garlicFlags);
}