#pragma once

#include "Clove/Graphics/DescriptorSet.hpp"

#include <vulkan/vulkan.h>

namespace clv::gfx::vk {
    class VKDescriptorSet : public DescriptorSet {
        //VARIABLES
    private:
        VkDevice device = VK_NULL_HANDLE;

        VkDescriptorSet descriptorSet = VK_NULL_HANDLE;

        //FUNCTIONS
    public:
        VKDescriptorSet() = delete;
        VKDescriptorSet(VkDevice device, VkDescriptorSet descriptorSet);

        VKDescriptorSet(const VKDescriptorSet& other) = delete;
        VKDescriptorSet(VKDescriptorSet&& other) noexcept;

        VKDescriptorSet& operator=(const VKDescriptorSet& other) = delete;
        VKDescriptorSet& operator=(VKDescriptorSet&& other) noexcept;

        ~VKDescriptorSet();

        void writeFrom(const GraphicsBuffer& buffer, const size_t offset, const size_t range, const uint32_t bindingSlot) override;
        void writeFrom(const Sampler& sampler, const GraphicsImageView& imageView, const ImageLayout layout, const uint32_t bindingSlot) override;

        VkDescriptorSet getDescriptorSet() const;
    };
}