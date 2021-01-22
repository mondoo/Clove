#pragma once

#include "Clove/Graphics/GhaImageView.hpp"

#include <vulkan/vulkan.h>

namespace garlic::clove {
    class VKImageView : public GhaImageView {
        //VARIABLES
    private:
        VkDevice device = VK_NULL_HANDLE;

        VkImageView imageView = VK_NULL_HANDLE;

        //FUNCTIONS
    public:
        VKImageView() = delete;
        VKImageView(VkDevice device, VkImageView imageView);

        VKImageView(VKImageView const& other) = delete;
        VKImageView(VKImageView&& other) noexcept;

        VKImageView& operator=(VKImageView const& other) = delete;
        VKImageView& operator=(VKImageView&& other) noexcept;

        ~VKImageView();

        inline VkImageView getImageView() const;

        static VkImageView create(VkDevice device, VkImage image, VkImageViewType viewType, VkFormat format, VkImageAspectFlags aspectFlags, uint32_t const baseLayer, uint32_t const layerCount);
    };
}

#include "VKImageView.inl"