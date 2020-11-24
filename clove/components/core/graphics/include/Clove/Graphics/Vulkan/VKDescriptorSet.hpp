#pragma once

#include "Clove/Graphics/DescriptorSet.hpp"

#include <vulkan/vulkan.h>

namespace garlic::clove {
    class VKDescriptorSet : public DescriptorSet {
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

        void map(GraphicsBuffer const &buffer, size_t const offset, size_t const range, uint32_t const bindingSlot) override;

        void map(GraphicsImageView const &imageView, Sampler const &sampler, GraphicsImage::Layout const layout, uint32_t const bindingSlot) override;
        void map(std::span<std::shared_ptr<GraphicsImageView>> imageViews, Sampler const &sampler, GraphicsImage::Layout const layout, uint32_t const bindingSlot) override;

        VkDescriptorSet getDescriptorSet() const;
    };
}