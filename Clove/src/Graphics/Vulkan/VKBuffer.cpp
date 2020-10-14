#include "Clove/Graphics/Vulkan/VKBuffer.hpp"

#include "Clove/Graphics/Vulkan/VKGraphicsResource.hpp"
#include "Clove/Graphics/Vulkan/VulkanTypes.hpp"
#include "Clove/Log.hpp"

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

        const bool isExclusive = this->descriptor.sharingMode == SharingMode::Exclusive;

        VkBufferCreateInfo createInfo{
            .sType                 = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO,
            .pNext                 = nullptr,
            .flags                 = 0,
            .size                  = descriptor.size,
            .usage                 = getUsageFlags(this->descriptor.usageFlags),
            .sharingMode           = isExclusive ? VK_SHARING_MODE_EXCLUSIVE : VK_SHARING_MODE_CONCURRENT,
            .queueFamilyIndexCount = isExclusive ? 0 : static_cast<uint32_t>(std::size(sharedQueueIndices)),
            .pQueueFamilyIndices   = isExclusive ? nullptr : std::data(sharedQueueIndices),
        };

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

    void VKBuffer::write(const void* data, const size_t offset, const size_t size) {
        GARLIC_ASSERT(descriptor.memoryType == MemoryType::SystemMemory, "{0}: Can only write to SystemMemory buffers", GARLIC_FUNCTION_NAME_PRETTY);

        void* cpuAccessibleMemory{ nullptr };

        vkMapMemory(device.get(), allocatedBlock->memory, allocatedBlock->offset + offset, size, 0, &cpuAccessibleMemory);
        memcpy(cpuAccessibleMemory, data, size);
        vkUnmapMemory(device.get(), allocatedBlock->memory);
    }

    void VKBuffer::read(void* data, const size_t offset, const size_t size) {
        GARLIC_ASSERT(descriptor.memoryType == MemoryType::SystemMemory, "{0}: Can only read from SystemMemory buffers", GARLIC_FUNCTION_NAME_PRETTY);

        void* cpuAccessibleMemory{ nullptr };

        vkMapMemory(device.get(), allocatedBlock->memory, allocatedBlock->offset + offset, size, 0, &cpuAccessibleMemory);
        memcpy(data, cpuAccessibleMemory, size);
        vkUnmapMemory(device.get(), allocatedBlock->memory);
    }

    VkBuffer VKBuffer::getBuffer() const {
        return buffer;
    }
}