#include "Clove/Graphics/Vulkan/VKDescriptorSet.hpp"

namespace clv::gfx::vk {
    VKDescriptorSet::VKDescriptorSet(VkDescriptorSet descriptorSet)
        : descriptorSet(descriptorSet) {
    }

    VkDescriptorSet VKDescriptorSet::getDescriptorSet() const {
        return descriptorSet;
    }
}