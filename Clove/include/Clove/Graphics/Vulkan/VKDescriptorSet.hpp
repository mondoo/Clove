#pragma once

//TODO: Remove
#include <vulkan/vulkan.h>

namespace clv::gfx::vk {
    class VKBuffer;
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

        void writeFrom(VKBuffer& buffer, const size_t offset, const size_t range, const uint32_t bindingSlot);

        VkDescriptorSet getDescriptorSet() const;
    };
}