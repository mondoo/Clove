#pragma once

//TODO: Remove
#include <vulkan/vulkan.h>

namespace clv::gfx {
    enum class DescriptorType{
        UniformBuffer,
    };

    struct DescriptorInfo {
        DescriptorType type;
        size_t count; //how many of this type we'll allocate
    };

    struct DescriptorPoolDescriptor{
        std::vector<DescriptorInfo> poolTypes;
        size_t maxSets; //how many could be allocated at a given time
    };
}

namespace clv::gfx::vk {
    //Used to allocate descriptor sets
    class VKDescriptorPool {
        //VARIABLES
    private:
        VkDevice device = VK_NULL_HANDLE;

        VkDescriptorPool pool = VK_NULL_HANDLE;

        //FUNCTIONS
    public:
        //TODO: Ctors
        VKDescriptorPool(VkDevice device, DescriptorPoolDescriptor descriptor);
        ~VKDescriptorPool();
    };
}