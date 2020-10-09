#include "Clove/Graphics/Vulkan/VKTransferQueue.hpp"

#include "Clove/Graphics/Vulkan/VKQueue.hpp"
#include "Clove/Graphics/Vulkan/VKTransferCommandBuffer.hpp"
#include "Clove/Log.hpp"
#include "Clove/Utils/Cast.hpp"

#include <Root/Definitions.hpp>

namespace clv::gfx::vk {
    VKTransferQueue::VKTransferQueue(DevicePointer device, QueueFamilyIndices queueFamilyIndices, CommandQueueDescriptor descriptor)
        : device(std::move(device))
        , queueFamilyIndices(std::move(queueFamilyIndices)) {
        const uint32_t familyIndex = *this->queueFamilyIndices.transferFamily;

        vkGetDeviceQueue(this->device.get(), familyIndex, 0, &queue);

        VkCommandPoolCreateInfo poolInfo{
            .sType            = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO,
            .pNext            = nullptr,
            .flags            = convertCommandPoolCreateFlags(descriptor.flags),
            .queueFamilyIndex = familyIndex,
        };

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

        VkCommandBufferAllocateInfo allocInfo{
            .sType              = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO,
            .commandPool        = commandPool,
            .level              = VK_COMMAND_BUFFER_LEVEL_PRIMARY,
            .commandBufferCount = 1,
        };

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

        VkSubmitInfo vkSubmitInfo{
            .sType                = VK_STRUCTURE_TYPE_SUBMIT_INFO,
            .waitSemaphoreCount   = 0,
            .pWaitSemaphores      = nullptr,
            .pWaitDstStageMask    = 0,
            .commandBufferCount   = static_cast<uint32_t>(commandBufferCount),
            .pCommandBuffers      = std::data(commandBuffers),
            .signalSemaphoreCount = 0,
            .pSignalSemaphores    = nullptr,
        };

        if(vkQueueSubmit(queue, 1, &vkSubmitInfo, VK_NULL_HANDLE) != VK_SUCCESS) {
            GARLIC_LOG(garlicLogContext, Log::Level::Error, "Failed to submit graphics command buffer(s)");
        }
        vkQueueWaitIdle(queue);//Because we're not using fences we'll just wait for the queue to complete for now
    }
}