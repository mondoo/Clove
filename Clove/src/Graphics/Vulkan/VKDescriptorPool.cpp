#include "Clove/Graphics/Vulkan/VKDescriptorPool.hpp"

namespace clv::gfx::vk {
    static VkDescriptorType getDescriptorType(DescriptorType garlicType) {
        switch(garlicType) {
            case DescriptorType::UniformBuffer:
                return VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
            default:
                CLV_ASSERT(false, "{0}: Unkown type", CLV_FUNCTION_NAME);
                return VK_DESCRIPTOR_TYPE_MAX_ENUM;
        }
    }

    VKDescriptorPool::VKDescriptorPool(VkDevice device, DescriptorPoolDescriptor descriptor)
        : device(device) {
        const size_t numDescriptorTypes = std::size(descriptor.poolTypes);
        std::vector<VkDescriptorPoolSize> poolSizes(numDescriptorTypes);
        for(size_t i = 0; i < numDescriptorTypes; ++i) {
            poolSizes[i].type            = getDescriptorType(descriptor.poolTypes[i].type);
            poolSizes[i].descriptorCount = descriptor.poolTypes[i].count;
        }

        VkDescriptorPoolCreateInfo createInfo{};
        createInfo.sType         = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
        createInfo.pNext         = nullptr;
        createInfo.flags         = 0;
        createInfo.maxSets       = descriptor.maxSets;
        createInfo.poolSizeCount = std::size(poolSizes);
        createInfo.pPoolSizes    = std::data(poolSizes);

        if(vkCreateDescriptorPool(device, &createInfo, nullptr, &pool) != VK_SUCCESS) {
            GARLIC_LOG(garlicLogContext, Log::Level::Error, "Failed to create descriptor pool");
        }
    }

    VKDescriptorPool::~VKDescriptorPool() {
        vkDestroyDescriptorPool(device, pool, nullptr);
    }
}