#include "Clove/Graphics/Vulkan/VulkanDescriptor.hpp"

#include <Clove/Log/Log.hpp>

namespace garlic::clove {
    VkDescriptorType getDescriptorType(DescriptorType garlicType) {
        switch(garlicType) {
            case DescriptorType::UniformBuffer:
                return VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
            case DescriptorType::StorageBuffer:
                return VK_DESCRIPTOR_TYPE_STORAGE_BUFFER;
            case DescriptorType::CombinedImageSampler:
                return VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
            default:
                CLOVE_ASSERT(false, "{0}: Unkown type", CLOVE_FUNCTION_NAME);
                return VK_DESCRIPTOR_TYPE_MAX_ENUM;
        }
    }
}