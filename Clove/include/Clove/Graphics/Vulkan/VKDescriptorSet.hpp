#pragma once

#include "Clove/Graphics/Vulkan/VulkanTypes.hpp"

//TODO: Remove
#include <vulkan/vulkan.h>

namespace clv::gfx::vk {
    class VKBuffer;
    class VKSampler;
    class VKImageView;
}

namespace clv::gfx::vk {
    class VKDescriptorSet{
        //VARIABLES
    private:
        VkDevice device = VK_NULL_HANDLE;

        VkDescriptorSet descriptorSet = VK_NULL_HANDLE;

        //FUNCTIONS
    public:
        //TODO: Ctors
        VKDescriptorSet(VkDevice device, VkDescriptorSet descriptorSet);

        void writeFrom(const VKBuffer& buffer, const size_t offset, const size_t range, const uint32_t bindingSlot);
        void writeFrom(const VKSampler& sampler, const VKImageView& imageView, const ImageLayout layout, const uint32_t bindingSlot);

        VkDescriptorSet getDescriptorSet() const;
    };
}