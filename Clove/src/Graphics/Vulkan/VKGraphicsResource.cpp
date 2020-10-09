#include "Clove/Graphics/Vulkan/VKGraphicsResource.hpp"

#include "Clove/Log.hpp"

#include <Root/Definitions.hpp>

namespace clv::gfx::vk {
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
}