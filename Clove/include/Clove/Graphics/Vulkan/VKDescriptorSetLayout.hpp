#pragma once

#include "Clove/Graphics/DescriptorSetLayout.hpp"
#include "Clove/Graphics/Vulkan/DevicePointer.hpp"

#include <vulkan/vulkan.h>

namespace clv::gfx::vk {
    class VKDescriptorSetLayout : public DescriptorSetLayout {
        //VARIABLES
    private:
        DevicePointer device;

        Descriptor descriptor;

        VkDescriptorSetLayout layout{ VK_NULL_HANDLE };

        //FUNCTIONS
    public:
        VKDescriptorSetLayout() = delete;
        VKDescriptorSetLayout(DevicePointer device, Descriptor descriptor);

        VKDescriptorSetLayout(const VKDescriptorSetLayout& other) = delete;
        VKDescriptorSetLayout(VKDescriptorSetLayout&& other) noexcept;

        VKDescriptorSetLayout& operator=(const VKDescriptorSetLayout& other) = delete;
        VKDescriptorSetLayout& operator=(VKDescriptorSetLayout&& other) noexcept;

        ~VKDescriptorSetLayout();

        inline const Descriptor& getDescriptor() const override;

        inline VkDescriptorSetLayout getLayout() const;
    };
}

#include "VKDescriptorSetLayout.inl"