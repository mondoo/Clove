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

    static uint32_t getMemoryTypeIndex(uint32_t typeFilter, VkMemoryPropertyFlags properties, VkPhysicalDevice physicalDevice) {
        VkPhysicalDeviceMemoryProperties memoryProperties{};
        vkGetPhysicalDeviceMemoryProperties(physicalDevice, &memoryProperties);

        for(uint32_t i = 0; i < memoryProperties.memoryTypeCount; ++i) {
            if(typeFilter & (1 << i) != 0 && (memoryProperties.memoryTypes[i].propertyFlags & properties) == properties) {
                return i;
            }
        }

        CLV_ASSERT(false, "{0}: Failed to find the specified index", CLV_FUNCTION_NAME);
        return -1;
    }

    static VkMemoryPropertyFlags getMemoryPropertyFlags(BufferMemoryProperties garlicProperties) {
        switch(garlicProperties) {
            case BufferMemoryProperties::DeviceLocal:
                return VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT;
            case BufferMemoryProperties::HostVisible:
                //Including HOST_COHERENT here as this makes mapping memory more simple
                return VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT;
            default:
                break;
        }
    }

    VKBuffer::VKBuffer(VkDevice device, VkPhysicalDevice physicalDevice, BufferDescriptor2 descriptor, const QueueFamilyIndices& familyIndices)
        : device(device)
        , descriptor(std::move(descriptor)) {
        std::array sharedQueueIndices = { *familyIndices.graphicsFamily, *familyIndices.transferFamily };

        VkBufferCreateInfo createInfo{};
        createInfo.sType                 = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
        createInfo.pNext                 = nullptr;
        createInfo.flags                 = 0;
        createInfo.size                  = descriptor.size;
        createInfo.usage                 = getUsageFlags(this->descriptor.usageFlags);
        if(this->descriptor.sharingMode == BufferSharingMode::Exclusive) {
            createInfo.sharingMode           = VK_SHARING_MODE_EXCLUSIVE;
            createInfo.queueFamilyIndexCount = 0;
            createInfo.pQueueFamilyIndices   = nullptr;
        } else {
            createInfo.sharingMode           = VK_SHARING_MODE_CONCURRENT;
            createInfo.queueFamilyIndexCount = std::size(sharedQueueIndices);
            createInfo.pQueueFamilyIndices   = std::data(sharedQueueIndices);
        }

        if(vkCreateBuffer(device, &createInfo, nullptr, &buffer) != VK_SUCCESS) {
            GARLIC_LOG(garlicLogContext, Log::Level::Error, "Failed to create buffer");
            return;
        }

        VkMemoryRequirements memoryRequirements{};
        vkGetBufferMemoryRequirements(device, buffer, &memoryRequirements);

        VkMemoryAllocateInfo allocInfo{};
        allocInfo.sType           = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
        allocInfo.pNext           = nullptr;
        allocInfo.allocationSize  = memoryRequirements.size;
        allocInfo.memoryTypeIndex = getMemoryTypeIndex(memoryRequirements.memoryTypeBits, getMemoryPropertyFlags(this->descriptor.memoryProperties), physicalDevice);//TODO: user specified flags

        if(vkAllocateMemory(device, &allocInfo, nullptr, &bufferMemory) != VK_SUCCESS) {
            GARLIC_LOG(garlicLogContext, Log::Level::Error, "Failed to allocate buffer memory");
            return;
        }

        vkBindBufferMemory(device, buffer, bufferMemory, 0);
    }

    VKBuffer::~VKBuffer() {
        vkDestroyBuffer(device, buffer, nullptr);
        vkFreeMemory(device, bufferMemory, nullptr);
    }

    void VKBuffer::map(const void* data, const size_t size) {
        CLV_ASSERT(size == descriptor.size, "{0}: Cannot map memory that is a different size to what the buffer expects", CLV_FUNCTION_NAME);

        void* cpuAccessibleMemory;

        vkMapMemory(device, bufferMemory, 0, descriptor.size, 0, &cpuAccessibleMemory);
        memcpy(cpuAccessibleMemory, data, descriptor.size);
        vkUnmapMemory(device, bufferMemory);
    }

    VkBuffer VKBuffer::getBuffer() const {
        return buffer;
    }
}