#include "Clove/Graphics/Vulkan/VKImage.hpp"

#include "Clove/Graphics/Vulkan/VKGraphicsResource.hpp"
#include "Clove/Graphics/Vulkan/VKImageView.hpp"
#include "Clove/Log.hpp"

#include <Root/Definitions.hpp>

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

    VKImage::VKImage(DevicePointer device, Descriptor descriptor, const QueueFamilyIndices& familyIndices, std::shared_ptr<MemoryAllocator> memoryAllocator)
        : device(std::move(device))
        , descriptor(std::move(descriptor))
        , memoryAllocator(std::move(memoryAllocator)) {
        std::array sharedQueueIndices = { *familyIndices.graphicsFamily, *familyIndices.transferFamily };

        const bool isExclusive = this->descriptor.sharingMode == SharingMode::Exclusive;

        VkImageCreateInfo createInfo{
            .sType                 = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO,
            .pNext                 = nullptr,
            .flags                 = 0,
            .imageType             = getImageType(this->descriptor.type),
            .format                = convertFormat(this->descriptor.format),
            .extent                = { this->descriptor.dimensions.x, this->descriptor.dimensions.y, 1 },
            .mipLevels             = 1,
            .arrayLayers           = 1,
            .samples               = VK_SAMPLE_COUNT_1_BIT,
            .tiling                = VK_IMAGE_TILING_OPTIMAL,
            .usage                 = getUsageFlags(this->descriptor.usageFlags),
            .sharingMode           = isExclusive ? VK_SHARING_MODE_EXCLUSIVE : VK_SHARING_MODE_CONCURRENT,
            .queueFamilyIndexCount = isExclusive ? 0 : static_cast<uint32_t>(std::size(sharedQueueIndices)),
            .pQueueFamilyIndices   = isExclusive ? nullptr : std::data(sharedQueueIndices),
            .initialLayout         = VK_IMAGE_LAYOUT_UNDEFINED,
        };

        if(vkCreateImage(this->device.get(), &createInfo, nullptr, &image) != VK_SUCCESS) {
            GARLIC_LOG(garlicLogContext, Log::Level::Error, "Failed to create image");
            return;
        }

        VkMemoryRequirements memoryRequirements{};
        vkGetImageMemoryRequirements(this->device.get(), image, &memoryRequirements);

        allocatedBlock = this->memoryAllocator->allocate(memoryRequirements, getMemoryPropertyFlags(this->descriptor.memoryType));

        vkBindImageMemory(this->device.get(), image, allocatedBlock->memory, allocatedBlock->offset);
    }

    VKImage::VKImage(VKImage&& other) noexcept = default;

    VKImage& VKImage::operator=(VKImage&& other) noexcept = default;

    VKImage::~VKImage() {
        vkDestroyImage(device.get(), image, nullptr);
        memoryAllocator->free(allocatedBlock);
    }

    std::unique_ptr<GraphicsImageView> VKImage::createView() const {
        const VkImageAspectFlags aspectFlags = descriptor.format == Format::D32_SFLOAT ? VK_IMAGE_ASPECT_DEPTH_BIT : VK_IMAGE_ASPECT_COLOR_BIT;
        return std::make_unique<VKImageView>(device.get(), VKImageView::create(device.get(), image, getImageViewType(descriptor.type), convertFormat(descriptor.format), aspectFlags));
    }

    VkImage VKImage::getImage() const {
        return image;
    }

    GraphicsImage::Format VKImage::convertFormat(VkFormat vulkanFormat) {
        switch(vulkanFormat) {
            //Formats supported by garlic
            case VK_FORMAT_R8G8B8A8_SRGB:
                return Format::R8G8B8A8_SRGB;
            case VK_FORMAT_B8G8R8A8_SRGB:
                return Format::B8G8R8A8_SRGB;
            case VK_FORMAT_B8G8R8A8_UNORM:
                return Format::B8G8R8A8_UNORM;
            case VK_FORMAT_D32_SFLOAT:
                return Format::D32_SFLOAT;
            default:
                GARLIC_ASSERT(false, "{0}: Format not supported by garlic", GARLIC_FUNCTION_NAME);
                return Format::Unkown;
        }
    }

    VkFormat VKImage::convertFormat(Format garlicFormat) {
        switch(garlicFormat) {
            case Format::Unkown:
                return VK_FORMAT_UNDEFINED;
            case Format::R8G8B8A8_SRGB:
                return VK_FORMAT_R8G8B8A8_SRGB;
            case Format::B8G8R8A8_SRGB:
                return VK_FORMAT_B8G8R8A8_SRGB;
            case Format::B8G8R8A8_UNORM:
                return VK_FORMAT_B8G8R8A8_UNORM;
            case Format::D32_SFLOAT:
                return VK_FORMAT_D32_SFLOAT;
            default:
                GARLIC_ASSERT(false, "{0}: Unkown format", GARLIC_FUNCTION_NAME);
                return VK_FORMAT_UNDEFINED;
        }
    }

    VkImageLayout VKImage::convertLayout(Layout garlicLayout) {
        switch(garlicLayout) {
            case Layout::Undefined:
                return VK_IMAGE_LAYOUT_UNDEFINED;
            case Layout::General:
                return VK_IMAGE_LAYOUT_GENERAL;
            case Layout::Present:
                return VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;
            case Layout::TransferDestinationOptimal:
                return VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL;
            case Layout::ShaderReadOnlyOptimal:
                return VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
            case Layout::ColourAttachmentOptimal:
                return VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;
            case Layout::DepthStencilAttachmentOptimal:
                return VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;
            case Layout::DepthStencilReadOnlyOptimal:
                return VK_IMAGE_LAYOUT_DEPTH_STENCIL_READ_ONLY_OPTIMAL;
            default:
                GARLIC_ASSERT(false, "{0}: Unkown image layout", GARLIC_FUNCTION_NAME);
                return VK_IMAGE_LAYOUT_UNDEFINED;
        }
    }
}