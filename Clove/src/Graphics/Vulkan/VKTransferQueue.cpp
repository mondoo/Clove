#include "Clove/Graphics/Vulkan/VKTransferQueue.hpp"

#include "Clove/Graphics/Vulkan/VKTransferCommandBuffer.hpp"
#include "Clove/Graphics/Vulkan/VulkanHelpers.hpp"

namespace clv::gfx::vk {
    VKTransferQueue::VKTransferQueue(DevicePointer device, QueueFamilyIndices queueFamilyIndices, CommandQueueDescriptor descriptor)
        : device(std::move(device))
        , queueFamilyIndices(std::move(queueFamilyIndices)) {
        const uint32_t familyIndex = *this->queueFamilyIndices.transferFamily;

        vkGetDeviceQueue(this->device.get(), familyIndex, 0, &queue);

        VkCommandPoolCreateInfo poolInfo{};
        poolInfo.sType            = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
        poolInfo.pNext            = nullptr;
        poolInfo.flags            = convertCommandPoolCreateFlags(descriptor.flags);
        poolInfo.queueFamilyIndex = familyIndex;

        if(vkCreateCommandPool(this->device.get(), &poolInfo, nullptr, &commandPool) != VK_SUCCESS) {
            GARLIC_LOG(garlicLogContext, Log::Level::Error, "Failed to create graphics command pool");
        }
    }

    VKTransferQueue::~VKTransferQueue() {
        vkDestroyCommandPool(device.get(), commandPool, nullptr);
    }

    std::unique_ptr<TransferCommandBuffer> VKTransferQueue::allocateCommandBuffer() {
        //TODO: Multiple command buffer allocation

        VkCommandBuffer commandBuffer;

        VkCommandBufferAllocateInfo allocInfo{};
        allocInfo.sType              = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
        allocInfo.commandPool        = commandPool;
        allocInfo.level              = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
        allocInfo.commandBufferCount = 1;

        if(vkAllocateCommandBuffers(device.get(), &allocInfo, &commandBuffer) != VK_SUCCESS) {
            GARLIC_LOG(garlicLogContext, Log::Level::Error, "Failed to allocate command buffer");
            return nullptr;
        }

        return std::make_unique<VKTransferCommandBuffer>(commandBuffer, queueFamilyIndices);
    }

    void VKTransferQueue::freeCommandBuffer(TransferCommandBuffer& buffer) {
        VkCommandBuffer buffers[] = { polyCast<VKTransferCommandBuffer>(&buffer)->getCommandBuffer() };
        vkFreeCommandBuffers(device.get(), commandPool, 1, buffers);
    }

    void VKTransferQueue::submit(const TransferSubmitInfo& submitInfo) {
        const size_t commandBufferCount = std::size(submitInfo.commandBuffers);
        std::vector<VkCommandBuffer> commandBuffers(commandBufferCount);
        for(size_t i = 0; i < commandBufferCount; i++) {
            commandBuffers[i] = polyCast<VKTransferCommandBuffer>(submitInfo.commandBuffers[i].get())->getCommandBuffer();
        }

        VkSubmitInfo vkSubmitInfo{};
        vkSubmitInfo.sType                = VK_STRUCTURE_TYPE_SUBMIT_INFO;
        vkSubmitInfo.waitSemaphoreCount   = 0;
        vkSubmitInfo.pWaitSemaphores      = nullptr;
        vkSubmitInfo.pWaitDstStageMask    = 0;
        vkSubmitInfo.commandBufferCount   = commandBufferCount;
        vkSubmitInfo.pCommandBuffers      = std::data(commandBuffers);
        vkSubmitInfo.signalSemaphoreCount = 0;
        vkSubmitInfo.pSignalSemaphores    = nullptr;

        if(vkQueueSubmit(queue, 1, &vkSubmitInfo, VK_NULL_HANDLE) != VK_SUCCESS) {
            GARLIC_LOG(garlicLogContext, Log::Level::Error, "Failed to submit graphics command buffer(s)");
        }
        vkQueueWaitIdle(queue);//Because we're not using fences we'll just wait for the queue to complete for now
    }
}