#include "Clove/Graphics/Vulkan/VKImage.hpp"

#include "Clove/Graphics/Vulkan/VulkanHelpers.hpp"

namespace clv::gfx::vk {
    static VkImageUsageFlags getUsageFlags(ImageUsageMode garlicUsageFlags) {
        VkBufferUsageFlags flags = 0;

        if((garlicUsageFlags & ImageUsageMode::TransferDestination) != 0) {
            flags |= VK_IMAGE_USAGE_TRANSFER_DST_BIT;
        }
        if((garlicUsageFlags & ImageUsageMode::Sampled) != 0) {
            flags |= VK_IMAGE_USAGE_SAMPLED_BIT;
        }
        if((garlicUsageFlags & ImageUsageMode::ColourAttachment) != 0) {
            flags |= VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;
        }
        if((garlicUsageFlags & ImageUsageMode::DepthStencilAttachment) != 0) {
            flags |= VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT;
        }

        return flags;
    }

    static VkImageType getImageType(ImageType garlicImageType) {
        switch(garlicImageType) {
            case ImageType::_2D:
                return VK_IMAGE_TYPE_2D;
            case ImageType::_3D:
                return VK_IMAGE_TYPE_3D;
            default:
                CLV_ASSERT(false, "{0}: Unhandled image type");
                return VK_IMAGE_TYPE_2D;
        }
    }

    VKImage::VKImage(VkDevice device, VkPhysicalDevice physicalDevice, ImageDescriptor descriptor, const QueueFamilyIndices& familyIndices)
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
        allocInfo.memoryTypeIndex = getMemoryTypeIndex(memoryRequirements.memoryTypeBits, getMemoryPropertyFlags(this->descriptor.memoryProperties), physicalDevice);//TODO: user specified flags

        if(vkAllocateMemory(device, &allocInfo, nullptr, &imageMemory) != VK_SUCCESS) {
            GARLIC_LOG(garlicLogContext, Log::Level::Error, "Failed to allocate buffer memory");
            return;
        }

        vkBindImageMemory(device, image, imageMemory, 0);
    }

    VKImage::~VKImage() {
        vkDestroyImage(device, image, nullptr);
        vkFreeMemory(device, imageMemory, nullptr);
    }

    VkImage VKImage::getImage() const{
        return image;
    }
}