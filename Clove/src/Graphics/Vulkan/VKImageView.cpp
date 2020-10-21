#include "Clove/Graphics/Vulkan/VKImageView.hpp"

#include "Clove/Log.hpp"

namespace clv::gfx::vk {
    VKImageView::VKImageView(VkDevice device, VkImageView imageView)
        : device(device)
        , imageView(imageView) {
    }

    VKImageView::VKImageView(VKImageView&& other) noexcept = default;

    VKImageView& VKImageView::operator=(VKImageView&& other) noexcept = default;

    VKImageView::~VKImageView() {
        vkDestroyImageView(device, imageView, nullptr);
    }

    VkImageView VKImageView::create(VkDevice device, VkImage image, VkImageViewType viewType, VkFormat format, VkImageAspectFlags aspectFlags, uint32_t const baseLayer, uint32_t const layerCount) {
        VkImageViewCreateInfo viewInfo{
            .sType      = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO,
            .pNext      = nullptr,
            .flags      = 0,
            .image      = image,
            .viewType   = viewType,
            .format     = format,
            .components = {
                .r = VK_COMPONENT_SWIZZLE_IDENTITY,
                .g = VK_COMPONENT_SWIZZLE_IDENTITY,
                .b = VK_COMPONENT_SWIZZLE_IDENTITY,
                .a = VK_COMPONENT_SWIZZLE_IDENTITY,
            },
            .subresourceRange = {
                .aspectMask     = aspectFlags,
                .baseMipLevel   = 0,
                .levelCount     = 1,
                .baseArrayLayer = baseLayer,
                .layerCount     = layerCount,
            }
        };

        VkImageView imageView;
        if(vkCreateImageView(device, &viewInfo, nullptr, &imageView) != VK_SUCCESS) {
            GARLIC_LOG(garlicLogContext, Log::Level::Error, "Failed to create texture image view");
            return VK_NULL_HANDLE;
        }

        return imageView;
    }
}