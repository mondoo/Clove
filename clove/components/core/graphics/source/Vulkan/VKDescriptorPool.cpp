#include "Clove/Graphics/Vulkan/VKDescriptorPool.hpp"

#include "Clove/Graphics/Vulkan/VKDescriptor.hpp"
#include "Clove/Graphics/Vulkan/VKDescriptorSet.hpp"
#include "Clove/Graphics/Vulkan/VKDescriptorSetLayout.hpp"

#include <Clove/Cast.hpp>
#include <Clove/Definitions.hpp>
#include <Clove/Log/Log.hpp>

namespace garlic::clove {
    static VkDescriptorPoolCreateFlags getDescriptorPoolFlags(DescriptorPool::Flag garlicFlag) {
        switch(garlicFlag) {
            case DescriptorPool::Flag::None:
                return 0;
            case DescriptorPool::Flag::FreeDescriptorSet:
                return VK_DESCRIPTOR_POOL_CREATE_FREE_DESCRIPTOR_SET_BIT;
            default:
                CLOVE_ASSERT(false, "{0} Unknown flag", CLOVE_FUNCTION_NAME);
        }
    }

    VKDescriptorPool::VKDescriptorPool(DevicePointer device, Descriptor descriptor)
        : device(std::move(device))
        , descriptor(std::move(descriptor)) {
        size_t const numDescriptorTypes = std::size(this->descriptor.poolTypes);
        std::vector<VkDescriptorPoolSize> poolSizes(numDescriptorTypes);
        for(size_t i = 0; i < numDescriptorTypes; ++i) {
            poolSizes[i].type            = getDescriptorType(this->descriptor.poolTypes[i].type);
            poolSizes[i].descriptorCount = this->descriptor.poolTypes[i].count;
        }

        VkDescriptorPoolCreateInfo createInfo{
            .sType         = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO,
            .pNext         = nullptr,
            .flags         = getDescriptorPoolFlags(this->descriptor.flag),
            .maxSets       = this->descriptor.maxSets,
            .poolSizeCount = static_cast<uint32_t>(std::size(poolSizes)),
            .pPoolSizes    = std::data(poolSizes),
        };

        if(vkCreateDescriptorPool(this->device.get(), &createInfo, nullptr, &pool) != VK_SUCCESS) {
            CLOVE_LOG(LOG_CATEGORY_CLOVE, LogLevel::Error, "Failed to create descriptor pool");
        }
    }

    VKDescriptorPool::VKDescriptorPool(VKDescriptorPool &&other) noexcept = default;

    VKDescriptorPool &VKDescriptorPool::operator=(VKDescriptorPool &&other) noexcept = default;

    VKDescriptorPool::~VKDescriptorPool() {
        vkDestroyDescriptorPool(device.get(), pool, nullptr);
    }

    std::shared_ptr<DescriptorSet> VKDescriptorPool::allocateDescriptorSets(std::shared_ptr<DescriptorSetLayout> const &layout) {
        return allocateDescriptorSets(std::vector{ layout })[0];
    }

    std::vector<std::shared_ptr<DescriptorSet>> VKDescriptorPool::allocateDescriptorSets(std::vector<std::shared_ptr<DescriptorSetLayout>> const &layouts) {
        size_t const numSets = std::size(layouts);

        std::vector<VkDescriptorSetLayout> vulkanLayouts(numSets);
        for(size_t i = 0; i < numSets; ++i) {
            vulkanLayouts[i] = polyCast<VKDescriptorSetLayout>(layouts[i].get())->getLayout();
        }

        std::vector<VkDescriptorSet> vulkanSets(numSets);

        VkDescriptorSetAllocateInfo allocInfo{
            .sType              = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO,
            .pNext              = nullptr,
            .descriptorPool     = pool,
            .descriptorSetCount = static_cast<uint32_t>(numSets),
            .pSetLayouts        = std::data(vulkanLayouts),
        };

        if(vkAllocateDescriptorSets(device.get(), &allocInfo, std::data(vulkanSets)) != VK_SUCCESS) {
            CLOVE_LOG(LOG_CATEGORY_CLOVE, LogLevel::Error, "Failed to allocate new descriptor sets");
            return {};
        }

        std::vector<std::shared_ptr<DescriptorSet>> descriptorSets(numSets);
        for(size_t i = 0; i < numSets; ++i) {
            descriptorSets[i] = std::make_shared<VKDescriptorSet>(device.get(), vulkanSets[i]);
        }

        return descriptorSets;
    }

    void VKDescriptorPool::freeDescriptorSets(std::shared_ptr<DescriptorSet> const &descriptorSet) {
        freeDescriptorSets(std::vector{ descriptorSet });
    }

    void VKDescriptorPool::freeDescriptorSets(std::vector<std::shared_ptr<DescriptorSet>> const &descriptorSets) {
        size_t const numSets = std::size(descriptorSets);

        std::vector<VkDescriptorSet> vulkanSets(numSets);
        for(size_t i = 0; i < numSets; ++i) {
            vulkanSets[i] = polyCast<VKDescriptorSet>(descriptorSets[i].get())->getDescriptorSet();
        }

        if(vkFreeDescriptorSets(device.get(), pool, numSets, std::data(vulkanSets)) != VK_SUCCESS) {
            CLOVE_LOG(LOG_CATEGORY_CLOVE, LogLevel::Error, "Failed to free descriptor sets");
        }
    }

    void VKDescriptorPool::reset() {
        vkResetDescriptorPool(device.get(), pool, 0);
    }
}