#pragma once

#include "Clove/Graphics/DescriptorPool.hpp"
#include "Clove/Graphics/Vulkan/DevicePointer.hpp"

#include <vulkan/vulkan.h>

namespace clv::gfx::vk {
    class VKDescriptorPool : public DescriptorPool {
        //VARIABLES
    private:
        DevicePointer device;

        Descriptor descriptor;

        VkDescriptorPool pool{ VK_NULL_HANDLE };

        //FUNCTIONS
    public:
        VKDescriptorPool() = delete;
        VKDescriptorPool(DevicePointer device, Descriptor descriptor);

        VKDescriptorPool(const VKDescriptorPool& other) = delete;
        VKDescriptorPool(VKDescriptorPool&& other) noexcept;

        VKDescriptorPool& operator=(const VKDescriptorPool& other) = delete;
        VKDescriptorPool& operator=(VKDescriptorPool&& other) noexcept;

        ~VKDescriptorPool();

        inline const Descriptor& getDescriptor() const override;

        std::vector<std::shared_ptr<DescriptorSet>> allocateDescriptorSets(const std::vector<std::shared_ptr<DescriptorSetLayout>>& layouts) override;
        void freeDescriptorSets(const std::vector<std::shared_ptr<DescriptorSet>>& descriptorSets) override;
    };
}

#include "VKDescriptorPool.inl"