#pragma once

#include "Clove/Graphics/Descriptor.hpp"

#include <vulkan/vulkan.h>

namespace garlic::clove{
    VkDescriptorType getDescriptorType(DescriptorType garlicType);
}