#pragma once

#include "Clove/Graphics/DescriptorSetLayout.hpp"

#include <vulkan/vulkan.h>

namespace clv::gfx::vk {
    class VKDescriptorSetLayout : public DescriptorSetLayout {
        //VARIABLES
    private:
        VkDevice device = VK_NULL_HANDLE;

        VkDescriptorSetLayout layout = VK_NULL_HANDLE;

        //FUNCTIONS
    public:
        VKDescriptorSetLayout() = delete;
        VKDescriptorSetLayout(VkDevice device, Descriptor descriptor);

        VKDescriptorSetLayout(const VKDescriptorSetLayout& other) = delete;
        VKDescriptorSetLayout(VKDescriptorSetLayout&& other) noexcept;

        VKDescriptorSetLayout& operator=(const VKDescriptorSetLayout& other) = delete;
        VKDescriptorSetLayout& operator=(VKDescriptorSetLayout&& other) noexcept;

        ~VKDescriptorSetLayout();

        VkDescriptorSetLayout getLayout() const;
    };
}