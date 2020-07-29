#include "Clove/Graphics/Vulkan/VKBuffer.hpp"

#include "Clove/Graphics/Vulkan/VulkanHelpers.hpp"
#include "Clove/Graphics/Vulkan/VulkanTypes.hpp"

namespace clv::gfx::vk {
    static VkBufferUsageFlags getUsageFlags(GraphicsBuffer::UsageMode garlicUsageFlags) {
        VkBufferUsageFlags flags = 0;

        if((garlicUsageFlags & GraphicsBuffer::UsageMode::TransferSource) != 0) {
            flags |= VK_BUFFER_USAGE_TRANSFER_SRC_BIT;
        }
        if((garlicUsageFlags & GraphicsBuffer::UsageMode::TransferDestination) != 0) {
            flags |= VK_BUFFER_USAGE_TRANSFER_DST_BIT;
        }
        if((garlicUsageFlags & GraphicsBuffer::UsageMode::VertexBuffer) != 0) {
            flags |= VK_BUFFER_USAGE_VERTEX_BUFFER_BIT;
        }
        if((garlicUsageFlags & GraphicsBuffer::UsageMode::IndexBuffer) != 0) {
            flags |= VK_BUFFER_USAGE_INDEX_BUFFER_BIT;
        }
        if((garlicUsageFlags & GraphicsBuffer::UsageMode::UniformBuffer) != 0) {
            flags |= VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT;
        }

        return flags;
    }

    VKBuffer::VKBuffer(VkDevice device, VkPhysicalDevice physicalDevice, Descriptor descriptor, const QueueFamilyIndices& familyIndices)
        : device(device)
        , descriptor(std::move(descriptor)) {
        std::array sharedQueueIndices = { *familyIndices.graphicsFamily, *familyIndices.transferFamily };

        VkBufferCreateInfo createInfo{};
        createInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
        createInfo.pNext = nullptr;
        createInfo.flags = 0;
        createInfo.size  = descriptor.size;
        createInfo.usage = getUsageFlags(this->descriptor.usageFlags);
        if(this->descriptor.sharingMode == SharingMode::Exclusive) {
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
        allocInfo.memoryTypeIndex = getMemoryTypeIndex(memoryRequirements.memoryTypeBits, getMemoryPropertyFlags(this->descriptor.memoryType), physicalDevice);//TODO: user specified flags

        if(vkAllocateMemory(device, &allocInfo, nullptr, &bufferMemory) != VK_SUCCESS) {
            GARLIC_LOG(garlicLogContext, Log::Level::Error, "Failed to allocate buffer memory");
            return;
        }

        vkBindBufferMemory(device, buffer, bufferMemory, 0);
    }

    VKBuffer::VKBuffer(VKBuffer&& other) noexcept = default;

    VKBuffer& VKBuffer::operator=(VKBuffer&& other) noexcept = default;

    VKBuffer::~VKBuffer() {
        vkDestroyBuffer(device, buffer, nullptr);
        vkFreeMemory(device, bufferMemory, nullptr);
    }

    void VKBuffer::map(const void* data, const size_t size) {
        GARLIC_ASSERT(size == descriptor.size, "{0}: Cannot map memory that is a different size to what the buffer expects", GARLIC_FUNCTION_NAME);

        void* cpuAccessibleMemory;

        vkMapMemory(device, bufferMemory, 0, descriptor.size, 0, &cpuAccessibleMemory);
        memcpy(cpuAccessibleMemory, data, descriptor.size);
        vkUnmapMemory(device, bufferMemory);
    }

    VkBuffer VKBuffer::getBuffer() const {
        return buffer;
    }
}