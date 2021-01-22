#pragma once

#include "Clove/Graphics/GhaDescriptorSet.hpp"

#include <vulkan/vulkan.h>

namespace garlic::clove {
    class VKDescriptorSet : public GhaDescriptorSet {
        //VARIABLES
    private:
        VkDevice device = VK_NULL_HANDLE;

        VkDescriptorSet descriptorSet = VK_NULL_HANDLE;

        //FUNCTIONS
    public:
        VKDescriptorSet() = delete;
        VKDescriptorSet(VkDevice device, VkDescriptorSet descriptorSet);

        VKDescriptorSet(VKDescriptorSet const &other) = delete;
        VKDescriptorSet(VKDescriptorSet &&other) noexcept;

        VKDescriptorSet &operator=(VKDescriptorSet const &other) = delete;
        VKDescriptorSet &operator=(VKDescriptorSet &&other) noexcept;

        ~VKDescriptorSet();

        void map(GhaBuffer const &buffer, size_t const offset, size_t const range, uint32_t const bindingSlot) override;

        void map(GhaImageView const &imageView, GhaSampler const &sampler, GhaImage::Layout const layout, uint32_t const bindingSlot) override;
        void map(std::span<std::shared_ptr<GhaImageView>> imageViews, GhaSampler const &sampler, GhaImage::Layout const layout, uint32_t const bindingSlot) override;

        VkDescriptorSet getDescriptorSet() const;
    };
}