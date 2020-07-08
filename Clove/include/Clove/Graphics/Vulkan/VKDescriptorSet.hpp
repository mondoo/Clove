#pragma once

//TODO: Remove
#include <vulkan/vulkan.h>

namespace clv::gfx::vk {
    class VKDescriptorSet{
        //VARIABLES
    private:
        VkDescriptorSet descriptorSet;

        //FUNCTIONS
    public:
        //TODO: Ctors
        VKDescriptorSet(VkDescriptorSet descriptorSet);

        VkDescriptorSet getDescriptorSet() const;
    };
}