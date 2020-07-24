#include "Clove/Graphics/Vulkan/VulkanHelpers.hpp"

namespace clv::gfx::vk {
    VkImageView createImageView(VkDevice device, VkImage image, VkFormat format, VkImageAspectFlags aspectFlags) {
        VkImageViewCreateInfo viewInfo{};
        viewInfo.sType                           = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
        viewInfo.image                           = image;
        viewInfo.viewType                        = VK_IMAGE_VIEW_TYPE_2D;
        viewInfo.format                          = format;
        viewInfo.subresourceRange.aspectMask     = aspectFlags;
        viewInfo.subresourceRange.baseMipLevel   = 0;
        viewInfo.subresourceRange.levelCount     = 1;
        viewInfo.subresourceRange.baseArrayLayer = 0;
        viewInfo.subresourceRange.layerCount     = 1;

        VkImageView imageView;
        if(vkCreateImageView(device, &viewInfo, nullptr, &imageView) != VK_SUCCESS) {
            GARLIC_LOG(garlicLogContext, Log::Level::Error, "Failed to create texture image view");
            return VK_NULL_HANDLE;
        }

        return imageView;
    }

    ImageFormat convertImageFormat(VkFormat vulkanFormat) {
        switch(vulkanFormat) {
            //Formats supported by garlic
            case VK_FORMAT_R8G8B8A8_SRGB:
                return ImageFormat::R8G8B8A8_SRGB;
            case VK_FORMAT_B8G8R8A8_SRGB:
                return ImageFormat::B8G8R8A8_SRGB;
            case VK_FORMAT_B8G8R8A8_UNORM:
                return ImageFormat::B8G8R8A8_UNORM;
            default:
                GARLIC_ASSERT(false, "Format not supported by garlic");
                return ImageFormat::Unkown;
        }
    }

    VkFormat convertImageFormat(ImageFormat garlicFormat) {
        switch(garlicFormat) {
            case ImageFormat::Unkown:
                return VK_FORMAT_UNDEFINED;
            case ImageFormat::R8G8B8A8_SRGB:
                return VK_FORMAT_R8G8B8A8_SRGB;
            case ImageFormat::B8G8R8A8_SRGB:
                return VK_FORMAT_B8G8R8A8_SRGB;
            case ImageFormat::B8G8R8A8_UNORM:
                return VK_FORMAT_B8G8R8A8_UNORM;
        }
    }

    Result convertResult(VkResult result) {
        switch(result) {
            case VK_SUCCESS:
                return Result::Success;
            case VK_SUBOPTIMAL_KHR:
                return Result::Success_SwapchainSuboptimal;
            case VK_ERROR_OUT_OF_DATE_KHR:
                return Result::Error_SwapchainOutOfDate;
            default:
                GARLIC_ASSERT(false, "Unkown result type");
                return Result::Unkown;
        }
    }

    VkFormat convertAttributeFormat(VertexAttributeFormat garlicFormat) {
        switch(garlicFormat) {
            case VertexAttributeFormat::R32_SFLOAT:
                return VK_FORMAT_R32_SFLOAT;
            case VertexAttributeFormat::R32G32_SFLOAT:
                return VK_FORMAT_R32G32_SFLOAT;
            case VertexAttributeFormat::R32G32B32_SFLOAT:
                return VK_FORMAT_R32G32B32_SFLOAT;
            case VertexAttributeFormat::R32G32B32A32_SFLOAT:
                return VK_FORMAT_R32G32B32A32_SFLOAT;
            default:
                GARLIC_ASSERT(false, "{0}: Unkown format passed", GARLIC_FUNCTION_NAME_PRETTY);
                return VK_FORMAT_UNDEFINED;
        }
    }

    VkDescriptorType getDescriptorType(DescriptorType garlicType) {
        switch(garlicType) {
            case DescriptorType::UniformBuffer:
                return VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
            case DescriptorType::CombinedImageSampler:
                return VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
            default:
                GARLIC_ASSERT(false, "{0}: Unkown type", GARLIC_FUNCTION_NAME);
                return VK_DESCRIPTOR_TYPE_MAX_ENUM;
        }
    }

    uint32_t getMemoryTypeIndex(uint32_t typeFilter, VkMemoryPropertyFlags properties, VkPhysicalDevice physicalDevice) {
        VkPhysicalDeviceMemoryProperties memoryProperties{};
        vkGetPhysicalDeviceMemoryProperties(physicalDevice, &memoryProperties);

        for(uint32_t i = 0; i < memoryProperties.memoryTypeCount; ++i) {
            if((typeFilter & (1 << i)) != 0 && (memoryProperties.memoryTypes[i].propertyFlags & properties) == properties) {
                return i;
            }
        }

        GARLIC_ASSERT(false, "{0}: Failed to find the specified index", GARLIC_FUNCTION_NAME);
        return -1;
    }

    VkMemoryPropertyFlags getMemoryPropertyFlags(MemoryProperties garlicProperties) {
        switch(garlicProperties) {
            case MemoryProperties::DeviceLocal:
                return VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT;
            case MemoryProperties::HostVisible:
                return VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT;//Including HOST_COHERENT here as this makes mapping memory more simple
            default:
                break;
        }
    }

    VkImageLayout convertImageLayout(ImageLayout garlicFormat) {
        switch(garlicFormat) {
            case ImageLayout::Undefined:
                return VK_IMAGE_LAYOUT_UNDEFINED;
            case ImageLayout::General:
                return VK_IMAGE_LAYOUT_GENERAL;
            case ImageLayout::TransferDestinationOptimal:
                return VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL;
            case ImageLayout::ShaderReadOnlyOptimal:
                return VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
            case ImageLayout::ColourAttachmentOptimal:
                return VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;
            case ImageLayout::DepthStencilAttachmentOptimal:
                return VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;
            case ImageLayout::DepthStencilReadOnlyOptimal:
                return VK_IMAGE_LAYOUT_DEPTH_STENCIL_READ_ONLY_OPTIMAL;
            default:
                GARLIC_ASSERT(false, "{0}: Unkown image layout", GARLIC_FUNCTION_NAME);
                return VK_IMAGE_LAYOUT_UNDEFINED;
        }
    }
}