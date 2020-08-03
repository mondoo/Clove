#include "Clove/Graphics/Vulkan/VulkanHelpers.hpp"

namespace clv::gfx::vk {
    VkImageView createImageView(VkDevice device, VkImage image, VkImageViewType viewType, VkFormat format, VkImageAspectFlags aspectFlags) {
        VkImageViewCreateInfo viewInfo{};
        viewInfo.sType                           = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
        viewInfo.pNext                           = nullptr;
        viewInfo.flags                           = 0;
        viewInfo.image                           = image;
        viewInfo.viewType                        = viewType;
        viewInfo.format                          = format;
        viewInfo.components.r                    = VK_COMPONENT_SWIZZLE_IDENTITY;
        viewInfo.components.g                    = VK_COMPONENT_SWIZZLE_IDENTITY;
        viewInfo.components.b                    = VK_COMPONENT_SWIZZLE_IDENTITY;
        viewInfo.components.a                    = VK_COMPONENT_SWIZZLE_IDENTITY;
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
            case VK_FORMAT_D32_SFLOAT:
                return ImageFormat::D32_SFLOAT;
            default:
                GARLIC_ASSERT(false, "{0}: Format not supported by garlic", GARLIC_FUNCTION_NAME);
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
            case ImageFormat::D32_SFLOAT:
                return VK_FORMAT_D32_SFLOAT;
            default:
                GARLIC_ASSERT(false, "{0}: Unkown format", GARLIC_FUNCTION_NAME);
                return VK_FORMAT_UNDEFINED;
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
        VkPhysicalDeviceMemoryProperties memoryType{};
        vkGetPhysicalDeviceMemoryProperties(physicalDevice, &memoryType);

        for(uint32_t i = 0; i < memoryType.memoryTypeCount; ++i) {
            if((typeFilter & (1 << i)) != 0 && (memoryType.memoryTypes[i].propertyFlags & properties) == properties) {
                return i;
            }
        }

        GARLIC_ASSERT(false, "{0}: Failed to find the specified index", GARLIC_FUNCTION_NAME);
        return -1;
    }

    VkMemoryPropertyFlags getMemoryPropertyFlags(MemoryType garlicProperties) {
        switch(garlicProperties) {
            case MemoryType::VideoMemory:
                return VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT;
            case MemoryType::SystemMemory:
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
            case ImageLayout::Present:
                return VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;
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

    VkAttachmentLoadOp convertLoadOp(LoadOperation garlicOperation) {
        switch(garlicOperation) {
            case LoadOperation::DontCare:
                return VK_ATTACHMENT_LOAD_OP_DONT_CARE;
            case LoadOperation::Clear:
                return VK_ATTACHMENT_LOAD_OP_CLEAR;
            default:
                GARLIC_ASSERT(false, "{0}: Unkown operation", GARLIC_FUNCTION_NAME);
                return VK_ATTACHMENT_LOAD_OP_DONT_CARE;
        }
    }

    VkAttachmentStoreOp convertStoreOp(StoreOperation garlicOperation) {
        switch(garlicOperation) {
            case StoreOperation::DontCare:
                return VK_ATTACHMENT_STORE_OP_DONT_CARE;
            case StoreOperation::Store:
                return VK_ATTACHMENT_STORE_OP_STORE;
            default:
                GARLIC_ASSERT(false, "{0}: Unkown operation", GARLIC_FUNCTION_NAME);
                return VK_ATTACHMENT_STORE_OP_DONT_CARE;
        }
    }

    VkPipelineStageFlags convertPipelineStage(PipelineStage garlicStage) {
        switch(garlicStage) {
            case PipelineStage::Top:
                return VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT;
            case PipelineStage::ColourAttachmentOutput:
                return VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
            default: 
                GARLIC_ASSERT(false, "{0}: Unkown pipeline stage", GARLIC_FUNCTION_NAME);
                return 0;
        }
    }

    VkAccessFlags convertAccessType(AccessType garlicAccess) {
        switch(garlicAccess) {
            case AccessType::None:
                return 0;
            case AccessType::ColourAttachmentWrite:
                return VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;
            default:
                GARLIC_ASSERT(false, "{0}: Unkown access type", GARLIC_FUNCTION_NAME);
                return 0;
        }
    }
}