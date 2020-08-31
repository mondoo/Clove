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

    VKBuffer::VKBuffer(DevicePointer device, Descriptor descriptor, const QueueFamilyIndices& familyIndices, std::shared_ptr<MemoryAllocator> memoryAllocator)
        : device(std::move(device))
        , descriptor(std::move(descriptor))
        , memoryAllocator(std::move(memoryAllocator)) {
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

        if(vkCreateBuffer(this->device.get(), &createInfo, nullptr, &buffer) != VK_SUCCESS) {
            GARLIC_LOG(garlicLogContext, Log::Level::Error, "Failed to create buffer");
            return;
        }

        VkMemoryRequirements memoryRequirements{};
        vkGetBufferMemoryRequirements(this->device.get(), buffer, &memoryRequirements);

        allocatedBlock = this->memoryAllocator->allocate(memoryRequirements, getMemoryPropertyFlags(this->descriptor.memoryType));

        vkBindBufferMemory(this->device.get(), buffer, allocatedBlock->memory, allocatedBlock->offset);
    }

    VKBuffer::VKBuffer(VKBuffer&& other) noexcept = default;

    VKBuffer& VKBuffer::operator=(VKBuffer&& other) noexcept = default;

    VKBuffer::~VKBuffer() {
        vkDestroyBuffer(device.get(), buffer, nullptr);
        memoryAllocator->free(allocatedBlock);
    }

    void VKBuffer::map(const void* data, const size_t size) {
        GARLIC_ASSERT(size <= allocatedBlock->size, "{0}: Cannot map memory that is bigger than the buffer has allocated", GARLIC_FUNCTION_NAME);

        void* cpuAccessibleMemory;

        vkMapMemory(device.get(), allocatedBlock->memory, allocatedBlock->offset, size, 0, &cpuAccessibleMemory);
        memcpy(cpuAccessibleMemory, data, size);
        vkUnmapMemory(device.get(), allocatedBlock->memory);
    }

    VkBuffer VKBuffer::getBuffer() const {
        return buffer;
    }
}