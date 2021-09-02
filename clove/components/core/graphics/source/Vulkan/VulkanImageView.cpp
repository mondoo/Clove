#include "Clove/Graphics/Vulkan/VulkanImageView.hpp"

#include "Clove/Graphics/Vulkan/VulkanLog.hpp"

namespace clove {
    VulkanImageView::VulkanImageView(GhaImage::Format viewedFormat, vec2ui viewedDimensions, VkDevice device, VkImageView imageView)
        : viewedFormat{ viewedFormat }
        , viewedDimensions{ std::move(viewedDimensions) }
        , device{ device }
        , imageView{ imageView } {
    }

    VulkanImageView::VulkanImageView(VulkanImageView &&other) noexcept = default;

    VulkanImageView &VulkanImageView::operator=(VulkanImageView &&other) noexcept = default;

    VulkanImageView::~VulkanImageView() {
        vkDestroyImageView(device, imageView, nullptr);
    }

    GhaImage::Format VulkanImageView::getImageFormat() const {
        return viewedFormat;
    }

    vec2ui const &VulkanImageView::getImageDimensions() const {
        return viewedDimensions;
    }

    VkImageViewType VulkanImageView::convertType(GhaImageView::Type garlicImageType, uint32_t const layerCount) {
        switch(garlicImageType) {
            case GhaImageView::Type::_2D:
                if(layerCount > 1u) {
                    return VK_IMAGE_VIEW_TYPE_2D_ARRAY;
                } else {
                    return VK_IMAGE_VIEW_TYPE_2D;
                }
            case GhaImageView::Type::_3D:
                return VK_IMAGE_VIEW_TYPE_3D;
            case GhaImageView::Type::Cube: {
                uint32_t constexpr cubeLayerCount{ 6 };
                if(layerCount > cubeLayerCount) {
                    return VK_IMAGE_VIEW_TYPE_CUBE_ARRAY;
                } else {
                    return VK_IMAGE_VIEW_TYPE_CUBE;
                }
            }
            default:
                CLOVE_ASSERT_MSG(false, "{0}: Unhandled image type");
                return VK_IMAGE_VIEW_TYPE_2D;
        }
    }
}