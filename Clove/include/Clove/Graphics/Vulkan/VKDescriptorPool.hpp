#pragma once

//TODO: Remove
#include <vulkan/vulkan.h>

namespace clv::gfx {
    enum class DescriptorType {
        UniformBuffer,
    };

    struct DescriptorInfo {
        DescriptorType type;
        uint32_t count;//number of this type to allocate
    };

    struct DescriptorPoolDescriptor {
        std::vector<DescriptorInfo> poolTypes;
        uint32_t maxSets;//the maximum amount of sets that can be allocated
    };
}

namespace clv::gfx::vk {
    class VKDescriptorSet;
    class VKDescriptorSetLayout;
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

        //Allocates a descriptor set for each layout provided
        std::vector<std::shared_ptr<VKDescriptorSet>> allocateDescriptorSets(const std::vector<std::shared_ptr<VKDescriptorSetLayout>>& layouts);
    };
}