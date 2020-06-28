#include "Clove/Graphics/Vulkan/VKBuffer.hpp"

namespace clv::gfx::vk {
    static VkBufferUsageFlags getUsageFlags(BufferUsageMode garlicUsageFlags) {
        VkBufferUsageFlags flags = 0;

        if((garlicUsageFlags & BufferUsageMode::TransferSource) != 0) {
            flags |= VK_BUFFER_USAGE_TRANSFER_SRC_BIT;
        }
        if((garlicUsageFlags & BufferUsageMode::TransferDestination) != 0) {
            flags |= VK_BUFFER_USAGE_TRANSFER_DST_BIT;
        }
        if((garlicUsageFlags & BufferUsageMode::VertexBuffer) != 0) {
            flags |= VK_BUFFER_USAGE_VERTEX_BUFFER_BIT;
        }

        return flags;
    }

    static VkSharingMode getSharingMode(BufferSharingMode garlicSharingMode) {
        switch(garlicSharingMode) {
            case BufferSharingMode::Exclusive:
                return VK_SHARING_MODE_EXCLUSIVE;
            case BufferSharingMode::Concurrent:
                return VK_SHARING_MODE_CONCURRENT;
            default:
                CLV_ASSERT(false, "{0}: Unhandled sharing mode", CLV_FUNCTION_NAME);
                return VK_SHARING_MODE_EXCLUSIVE;
        }
    }

    VKBuffer::VKBuffer(VkDevice device, BufferDescriptor descriptor)
        : device(device) {
        VkBufferCreateInfo createInfo{};
        createInfo.sType                 = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
        createInfo.pNext                 = nullptr;
        createInfo.flags                 = 0;
        createInfo.size                  = descriptor.size;
        createInfo.usage                 = getUsageFlags(descriptor.usageFlags);
        createInfo.sharingMode           = getSharingMode(descriptor.sharingMode);
        createInfo.queueFamilyIndexCount = 0;
        createInfo.pQueueFamilyIndices   = nullptr;

        if(vkCreateBuffer(device, &createInfo, nullptr, &buffer) != VK_SUCCESS) {
            GARLIC_LOG(garlicLogContext, Log::Level::Error, "Failed to create buffer");
        }
    }

    VKBuffer::~VKBuffer() {
        vkDestroyBuffer(device, buffer, nullptr);
    }

    VkBuffer VKBuffer::getBuffer() const {
        return buffer;
    }
}