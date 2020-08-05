#include "Clove/Graphics/Vulkan/VKDescriptorPool.hpp"

#include "Clove/Graphics/Vulkan/VKDescriptorSet.hpp"
#include "Clove/Graphics/Vulkan/VKDescriptorSetLayout.hpp"
#include "Clove/Graphics/Vulkan/VulkanHelpers.hpp"

namespace clv::gfx::vk {
    VKDescriptorPool::VKDescriptorPool(VkDevice device, Descriptor descriptor)
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

    VKDescriptorPool::VKDescriptorPool(VKDescriptorPool&& other) noexcept = default;

    VKDescriptorPool& VKDescriptorPool::operator=(VKDescriptorPool&& other) noexcept = default;

    VKDescriptorPool::~VKDescriptorPool() {
        vkDestroyDescriptorPool(device, pool, nullptr);
    }

    std::vector<std::shared_ptr<DescriptorSet>> VKDescriptorPool::allocateDescriptorSets(const std::vector<std::shared_ptr<DescriptorSetLayout>>& layouts) {
        const size_t numSets = std::size(layouts);

        std::vector<VkDescriptorSetLayout> vulkanLayouts(numSets);
        for(size_t i = 0; i < numSets; ++i) {
            vulkanLayouts[i] = polyCast<VKDescriptorSetLayout>(layouts[i].get())->getLayout();
        }

        std::vector<VkDescriptorSet> vulkanSets(numSets);

        VkDescriptorSetAllocateInfo allocInfo{};
        allocInfo.sType              = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
        allocInfo.pNext              = nullptr;
        allocInfo.descriptorPool     = pool;
        allocInfo.descriptorSetCount = static_cast<uint32_t>(numSets);
        allocInfo.pSetLayouts        = std::data(vulkanLayouts);

        if(vkAllocateDescriptorSets(device, &allocInfo, std::data(vulkanSets)) != VK_SUCCESS) {
            GARLIC_LOG(garlicLogContext, Log::Level::Error, "Failed to allocate new descriptor sets");
            return {};
        }

        std::vector<std::shared_ptr<DescriptorSet>> descriptorSets(numSets);
        for(size_t i = 0; i < numSets; ++i) {
            descriptorSets[i] = std::make_shared<VKDescriptorSet>(device, vulkanSets[i]);
        }

        return descriptorSets;
    }

    void VKDescriptorPool::freeDescriptorSets(const std::vector<std::shared_ptr<DescriptorSet>>& descriptorSets) {
        const size_t numSets = std::size(descriptorSets);

        std::vector<VkDescriptorSet> vulkanSets(numSets);
        for(size_t i = 0; i < numSets; ++i) {
            vulkanSets[i] = polyCast<VKDescriptorSet>(descriptorSets[i].get())->getDescriptorSet();
        }

        if(vkFreeDescriptorSets(device, pool, numSets, std::data(vulkanSets)) != VK_SUCCESS) {
            GARLIC_LOG(garlicLogContext, Log::Level::Error, "Failed to free descriptor sets");
        }
    }
}