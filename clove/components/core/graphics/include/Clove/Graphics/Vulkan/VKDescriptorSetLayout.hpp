#pragma once

#include "Clove/Graphics/GhaDescriptorSetLayout.hpp"
#include "Clove/Graphics/Vulkan/DevicePointer.hpp"

#include <vulkan/vulkan.h>

namespace garlic::clove {
    class VKDescriptorSetLayout : public GhaDescriptorSetLayout {
        //VARIABLES
    private:
        DevicePointer device;

        Descriptor descriptor;

        VkDescriptorSetLayout layout{ VK_NULL_HANDLE };

        //FUNCTIONS
    public:
        VKDescriptorSetLayout() = delete;
        VKDescriptorSetLayout(DevicePointer device, VkDescriptorSetLayout layout, Descriptor descriptor);

        VKDescriptorSetLayout(VKDescriptorSetLayout const &other) = delete;
        VKDescriptorSetLayout(VKDescriptorSetLayout &&other) noexcept;

        VKDescriptorSetLayout &operator=(VKDescriptorSetLayout const &other) = delete;
        VKDescriptorSetLayout &operator =(VKDescriptorSetLayout &&other) noexcept;

        ~VKDescriptorSetLayout();

        inline Descriptor const &getDescriptor() const override;

        inline VkDescriptorSetLayout getLayout() const;
    };
}

#include "VKDescriptorSetLayout.inl"