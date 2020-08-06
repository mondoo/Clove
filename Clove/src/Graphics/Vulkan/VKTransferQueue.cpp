#include "Clove/Graphics/Vulkan/VKTransferQueue.hpp"

namespace clv::gfx::vk {
    std::unique_ptr<VKTransferCommandBuffer> VKTransferQueue::allocateCommandBuffer() {
        //TODO: Multiple command buffer allocation

        VkCommandBuffer commandBuffer;

        VkCommandBufferAllocateInfo allocInfo{};
        allocInfo.sType              = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
        allocInfo.commandPool        = commandPool;
        allocInfo.level              = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
        allocInfo.commandBufferCount = 1;

        if(vkAllocateCommandBuffers(device, &allocInfo, &commandBuffer) != VK_SUCCESS) {
            GARLIC_LOG(garlicLogContext, Log::Level::Error, "Failed to allocate command buffer");
            return nullptr;
        }

        return std::make_unique<VKTransferCommandBuffer>(commandBuffer);
    }

    void VKTransferQueue::freeCommandBuffer(VKTransferCommandBuffer& buffer) {
        VkCommandBuffer buffers[] = { buffer.getCommandBuffer() };
        vkFreeCommandBuffers(device, commandPool, 1, buffers);
    }

    void VKTransferQueue::submit(const TransferSubmitInfo& submitInfo) {
        const size_t commandBufferCount = std::size(submitInfo.commandBuffers);
        std::vector<VkCommandBuffer> commandBuffers(commandBufferCount);
        for(size_t i = 0; i < commandBufferCount; i++) {
            commandBuffers[i] = submitInfo.commandBuffers[i]->getCommandBuffer();
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