#include "Clove/Graphics/Vulkan/VulkanHelpers.hpp"

#include "Clove/Log.hpp"

#include <Root/Definitions.hpp>

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

    VkShaderStageFlags convertShaderStage(Shader::Stage stage) {
        VkShaderStageFlags vkStage{ 0 };
        if((stage & Shader::Stage::Vertex) != 0) {
            vkStage |= VK_SHADER_STAGE_VERTEX_BIT;
        }
        if((stage & Shader::Stage::Pixel) != 0) {
            vkStage |= VK_SHADER_STAGE_FRAGMENT_BIT;
        }
        return vkStage;
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

    VkAccessFlags convertAccessFlags(AccessFlags garlicAccess) {
        VkAccessFlags flags = 0;
        uint32_t total      = 0;

        if((garlicAccess & AccessFlags::TransferWrite) != 0) {
            flags |= VK_ACCESS_TRANSFER_WRITE_BIT;
            total += static_cast<AccessFlagsType>(AccessFlags::TransferWrite);
        }
        if((garlicAccess & AccessFlags::ShaderRead) != 0) {
            flags |= VK_ACCESS_SHADER_READ_BIT;
            total += static_cast<AccessFlagsType>(AccessFlags::ShaderRead);
        }
        if((garlicAccess & AccessFlags::ColourAttachmentWrite) != 0) {
            flags |= VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;
            total += static_cast<AccessFlagsType>(AccessFlags::ColourAttachmentWrite);
        }
        if((garlicAccess & AccessFlags::DepthStencilAttachmentRead) != 0) {
            flags |= VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_READ_BIT;
            total += static_cast<AccessFlagsType>(AccessFlags::DepthStencilAttachmentRead);
        }
        if((garlicAccess & AccessFlags::DepthStencilAttachmentWrite) != 0) {
            flags |= VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT;
            total += static_cast<AccessFlagsType>(AccessFlags::DepthStencilAttachmentWrite);
        }

        if(total != static_cast<AccessFlagsType>(garlicAccess)) {
            GARLIC_ASSERT(false, "{0}: Unhandled access type", GARLIC_FUNCTION_NAME);
        }

        return flags;
    }

    uint32_t getQueueFamilyIndex(QueueType type, const QueueFamilyIndices& indices) {
        switch(type) {
            case QueueType::None:
                return VK_QUEUE_FAMILY_IGNORED;
            case QueueType::Graphics:
                return *indices.graphicsFamily;
            case QueueType::Transfer:
                return *indices.transferFamily;
            default:
                GARLIC_ASSERT(false, "{0}: Unkown queue type", GARLIC_FUNCTION_NAME);
                return VK_QUEUE_FAMILY_IGNORED;
        }
    }

    VkCommandPoolCreateFlags convertCommandPoolCreateFlags(QueueFlags garlicFlags) {
        switch(garlicFlags) {
            case QueueFlags::None:
                return 0;
            case QueueFlags::Transient:
                return VK_COMMAND_POOL_CREATE_TRANSIENT_BIT;
            case QueueFlags::ReuseBuffers:
                return VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;
            default:
                GARLIC_ASSERT(false, "{0}: Unkown queue flag", GARLIC_FUNCTION_NAME);
                return 0;
        }
    }

    VkCommandBufferUsageFlags getCommandBufferUsageFlags(CommandBufferUsage garlicUsage) {
        switch(garlicUsage) {
            case clv::gfx::CommandBufferUsage::Default:
                return 0;
            case clv::gfx::CommandBufferUsage::OneTimeSubmit:
                return VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;
            default:
                GARLIC_ASSERT(false, "{0}: Unkown usage type", GARLIC_FUNCTION_NAME);
                return 0;
        }
    }

    VkIndexType getIndexType(IndexType garlicType) {
        switch(garlicType) {
            case IndexType::Uint16:
                return VK_INDEX_TYPE_UINT16;
            default:
                GARLIC_ASSERT(false, "{0}: Unkown index type", GARLIC_FUNCTION_NAME);
                return VK_INDEX_TYPE_UINT16;
        }
    }
}