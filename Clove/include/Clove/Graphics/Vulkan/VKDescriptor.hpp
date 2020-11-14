#pragma once

#include "Clove/Graphics/Descriptor.hpp"

#include <vulkan/vulkan.h>

namespace clv::gfx::vk {
    VkDescriptorType getDescriptorType(DescriptorType garlicType);
}