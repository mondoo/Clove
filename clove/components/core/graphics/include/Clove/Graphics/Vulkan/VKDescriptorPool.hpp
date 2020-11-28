#pragma once

#include "Clove/Graphics/DescriptorPool.hpp"
#include "Clove/Graphics/Vulkan/DevicePointer.hpp"

#include <vulkan/vulkan.h>

namespace garlic::clove {
    class VKDescriptorPool : public DescriptorPool {
        //VARIABLES
    private:
        DevicePointer device;
        VkDescriptorPool pool{ VK_NULL_HANDLE };

        Descriptor descriptor;

        //FUNCTIONS
    public:
        VKDescriptorPool() = delete;
        VKDescriptorPool(DevicePointer device, VkDescriptorPool pool, Descriptor descriptor);

        VKDescriptorPool(VKDescriptorPool const &other) = delete;
        VKDescriptorPool(VKDescriptorPool &&other) noexcept;

        VKDescriptorPool &operator=(VKDescriptorPool const &other) = delete;
        VKDescriptorPool &operator=(VKDescriptorPool &&other) noexcept;

        ~VKDescriptorPool();

        inline Descriptor const &getDescriptor() const override;

        std::shared_ptr<DescriptorSet> allocateDescriptorSets(std::shared_ptr<DescriptorSetLayout> const &layout) override;
        std::vector<std::shared_ptr<DescriptorSet>> allocateDescriptorSets(std::vector<std::shared_ptr<DescriptorSetLayout>> const &layouts) override;

        void freeDescriptorSets(std::shared_ptr<DescriptorSet> const &descriptorSet) override;
        void freeDescriptorSets(std::vector<std::shared_ptr<DescriptorSet>> const &descriptorSets) override;

        void reset() override;
    };
}

#include "VKDescriptorPool.inl"