#include "Clove/Graphics/Vulkan/VKImage.hpp"

#include "Clove/Graphics/Vulkan/VKImageView.hpp"
#include "Clove/Graphics/Vulkan/VulkanHelpers.hpp"

namespace clv::gfx::vk {
    static VkImageUsageFlags getUsageFlags(GraphicsImage::UsageMode garlicUsageFlags) {
        VkBufferUsageFlags flags = 0;

        if((garlicUsageFlags & GraphicsImage::UsageMode::TransferDestination) != 0) {
            flags |= VK_IMAGE_USAGE_TRANSFER_DST_BIT;
        }
        if((garlicUsageFlags & GraphicsImage::UsageMode::Sampled) != 0) {
            flags |= VK_IMAGE_USAGE_SAMPLED_BIT;
        }
        if((garlicUsageFlags & GraphicsImage::UsageMode::ColourAttachment) != 0) {
            flags |= VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;
        }
        if((garlicUsageFlags & GraphicsImage::UsageMode::DepthStencilAttachment) != 0) {
            flags |= VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT;
        }

        return flags;
    }

    static VkImageType getImageType(GraphicsImage::Type garlicImageType) {
        switch(garlicImageType) {
            case GraphicsImage::Type::_2D:
                return VK_IMAGE_TYPE_2D;
            case GraphicsImage::Type::_3D:
                return VK_IMAGE_TYPE_3D;
            default:
                GARLIC_ASSERT(false, "{0}: Unhandled image type");
                return VK_IMAGE_TYPE_2D;
        }
    }

    static VkImageViewType getImageViewType(GraphicsImage::Type garlicImageType) {
        switch(garlicImageType) {
            case GraphicsImage::Type::_2D:
                return VK_IMAGE_VIEW_TYPE_2D;
            case GraphicsImage::Type::_3D:
                return VK_IMAGE_VIEW_TYPE_3D;
            default:
                GARLIC_ASSERT(false, "{0}: Unhandled image type");
                return VK_IMAGE_VIEW_TYPE_2D;
        }
    }

    VKImage::VKImage(VkDevice device, VkPhysicalDevice physicalDevice, Descriptor descriptor, const QueueFamilyIndices& familyIndices)
        : device(device)
        , descriptor(std::move(descriptor)) {
        std::array sharedQueueIndices = { *familyIndices.graphicsFamily, *familyIndices.transferFamily };

        VkImageCreateInfo createInfo{};
        createInfo.sType       = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
        createInfo.pNext       = nullptr;
        createInfo.flags       = 0;
        createInfo.imageType   = getImageType(this->descriptor.type);
        createInfo.format      = convertImageFormat(this->descriptor.format);
        createInfo.extent      = { this->descriptor.dimensions.x, this->descriptor.dimensions.y, 1 };
        createInfo.mipLevels   = 1;
        createInfo.arrayLayers = 1;
        createInfo.samples     = VK_SAMPLE_COUNT_1_BIT;
        createInfo.tiling      = VK_IMAGE_TILING_OPTIMAL;
        createInfo.usage       = getUsageFlags(this->descriptor.usageFlags);
        if(this->descriptor.sharingMode == SharingMode::Exclusive) {
            createInfo.sharingMode           = VK_SHARING_MODE_EXCLUSIVE;
            createInfo.queueFamilyIndexCount = 0;
            createInfo.pQueueFamilyIndices   = nullptr;
        } else {
            createInfo.sharingMode           = VK_SHARING_MODE_CONCURRENT;
            createInfo.queueFamilyIndexCount = std::size(sharedQueueIndices);
            createInfo.pQueueFamilyIndices   = std::data(sharedQueueIndices);
        }
        createInfo.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;

        if(vkCreateImage(device, &createInfo, nullptr, &image) != VK_SUCCESS) {
            GARLIC_LOG(garlicLogContext, Log::Level::Error, "Failed to create image");
            return;
        }

        VkMemoryRequirements memoryRequirements{};
        vkGetImageMemoryRequirements(device, image, &memoryRequirements);

        VkMemoryAllocateInfo allocInfo{};
        allocInfo.sType           = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
        allocInfo.pNext           = nullptr;
        allocInfo.allocationSize  = memoryRequirements.size;
        allocInfo.memoryTypeIndex = getMemoryTypeIndex(memoryRequirements.memoryTypeBits, getMemoryPropertyFlags(this->descriptor.memoryType), physicalDevice);//TODO: user specified flags

        if(vkAllocateMemory(device, &allocInfo, nullptr, &imageMemory) != VK_SUCCESS) {
            GARLIC_LOG(garlicLogContext, Log::Level::Error, "Failed to allocate buffer memory");
            return;
        }

        vkBindImageMemory(device, image, imageMemory, 0);
    }

    VKImage::VKImage(VKImage&& other) noexcept = default;

    VKImage& VKImage::operator=(VKImage&& other) noexcept = default;

    VKImage::~VKImage() {
        vkDestroyImage(device, image, nullptr);
        vkFreeMemory(device, imageMemory, nullptr);
    }

    std::unique_ptr<GraphicsImageView> VKImage::createView() const {
        VkImageViewCreateInfo viewInfo{};
        viewInfo.sType                           = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
        viewInfo.pNext                           = nullptr;
        viewInfo.flags                           = 0;
        viewInfo.image                           = image;
        viewInfo.viewType                        = getImageViewType(descriptor.type);
        viewInfo.format                          = convertImageFormat(descriptor.format);
        viewInfo.components.r                    = VK_COMPONENT_SWIZZLE_IDENTITY;
        viewInfo.components.g                    = VK_COMPONENT_SWIZZLE_IDENTITY;
        viewInfo.components.b                    = VK_COMPONENT_SWIZZLE_IDENTITY;
        viewInfo.components.a                    = VK_COMPONENT_SWIZZLE_IDENTITY;
        viewInfo.subresourceRange.aspectMask     = descriptor.format == ImageFormat::D32_SFLOAT ? VK_IMAGE_ASPECT_DEPTH_BIT : VK_IMAGE_ASPECT_COLOR_BIT;//TODO: Handle depth / stencil
        viewInfo.subresourceRange.baseMipLevel   = 0;
        viewInfo.subresourceRange.levelCount     = 1;
        viewInfo.subresourceRange.baseArrayLayer = 0;
        viewInfo.subresourceRange.layerCount     = 1;

        VkImageView imageView;
        GARLIC_ASSERT(vkCreateImageView(device, &viewInfo, nullptr, &imageView) == VK_SUCCESS, "{0}: Unable to create imageview", GARLIC_FUNCTION_NAME);

        return std::make_unique<VKImageView>(device, imageView);
    }

    VkImage VKImage::getImage() const {
        return image;
    }
}