#pragma once

#include "Clove/Graphics/GhaDescriptorPool.hpp"
#include "Clove/Graphics/Vulkan/DevicePointer.hpp"

#include <vulkan/vulkan.h>

namespace garlic::clove {
    class VKDescriptorPool : public GhaDescriptorPool {
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

        std::shared_ptr<GhaDescriptorSet> allocateDescriptorSets(std::shared_ptr<GhaDescriptorSetLayout> const &layout) override;
        std::vector<std::shared_ptr<GhaDescriptorSet>> allocateDescriptorSets(std::vector<std::shared_ptr<GhaDescriptorSetLayout>> const &layouts) override;

        void freeDescriptorSets(std::shared_ptr<GhaDescriptorSet> const &descriptorSet) override;
        void freeDescriptorSets(std::vector<std::shared_ptr<GhaDescriptorSet>> const &descriptorSets) override;

        void reset() override;
    };
}

#include "VKDescriptorPool.inl"