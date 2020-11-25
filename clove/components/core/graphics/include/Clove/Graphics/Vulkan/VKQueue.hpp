#pragma once

#include "Clove/Graphics/Queue.hpp"

#include <vulkan/vk_icd.h>

namespace garlic::clove {
    VkCommandPoolCreateFlags convertCommandPoolCreateFlags(QueueFlags garlicFlags);
}