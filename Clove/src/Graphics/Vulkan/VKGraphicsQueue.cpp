#include "Clove/Graphics/Vulkan/VKGraphicsQueue.hpp"

#include "Clove/Graphics/Vulkan/VKFence.hpp"
#include "Clove/Graphics/Vulkan/VKGraphicsCommandBuffer.hpp"
#include "Clove/Graphics/Vulkan/VKPipelineObject.hpp"
#include "Clove/Graphics/Vulkan/VKQueue.hpp"
#include "Clove/Graphics/Vulkan/VKSemaphore.hpp"
#include "Clove/Utils/Cast.hpp"

#include <Root/Definitions.hpp>
#include <Root/Log/Log.hpp>

namespace clv::gfx::vk {
    VKGraphicsQueue::VKGraphicsQueue(DevicePointer device, QueueFamilyIndices queueFamilyIndices, CommandQueueDescriptor descriptor)
        : device(std::move(device))
        , queueFamilyIndices(std::move(queueFamilyIndices)) {
        uint32_t const familyIndex{ *this->queueFamilyIndices.graphicsFamily };

        vkGetDeviceQueue(this->device.get(), familyIndex, 0, &queue);

        VkCommandPoolCreateInfo poolInfo{
            .sType            = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO,
            .pNext            = nullptr,
            .flags            = convertCommandPoolCreateFlags(descriptor.flags),
            .queueFamilyIndex = familyIndex,
        };

        if(vkCreateCommandPool(this->device.get(), &poolInfo, nullptr, &commandPool) != VK_SUCCESS) {
            GARLIC_LOG(garlicLogContext, garlic::LogLevel::Error, "Failed to create graphics command pool");
        }
    }

    VKGraphicsQueue::VKGraphicsQueue(VKGraphicsQueue &&other) noexcept = default;

    VKGraphicsQueue &VKGraphicsQueue::operator=(VKGraphicsQueue &&other) noexcept = default;

    VKGraphicsQueue::~VKGraphicsQueue() {
        vkDestroyCommandPool(device.get(), commandPool, nullptr);
    }

    std::unique_ptr<GraphicsCommandBuffer> VKGraphicsQueue::allocateCommandBuffer() {
        //TODO: Multiple command buffer allocation

        VkCommandBuffer commandBuffer;

        VkCommandBufferAllocateInfo allocInfo{
            .sType              = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO,
            .commandPool        = commandPool,
            .level              = VK_COMMAND_BUFFER_LEVEL_PRIMARY,
            .commandBufferCount = 1,
        };

        if(vkAllocateCommandBuffers(device.get(), &allocInfo, &commandBuffer) != VK_SUCCESS) {
            GARLIC_LOG(garlicLogContext, garlic::LogLevel::Error, "Failed to allocate command buffer");
            return nullptr;
        }

        return std::make_unique<VKGraphicsCommandBuffer>(commandBuffer, queueFamilyIndices);
    }

    void VKGraphicsQueue::freeCommandBuffer(GraphicsCommandBuffer &buffer) {
        VkCommandBuffer buffers[] = { polyCast<VKGraphicsCommandBuffer>(&buffer)->getCommandBuffer() };
        vkFreeCommandBuffers(device.get(), commandPool, 1, buffers);
    }

    void VKGraphicsQueue::submit(GraphicsSubmitInfo const &submitInfo, Fence const *fence) {
        //Wait semaphores / stages
        size_t const waitSemaphoreCount{ std::size(submitInfo.waitSemaphores) };
        std::vector<VkSemaphore> waitSemaphores(waitSemaphoreCount);
        std::vector<VkPipelineStageFlags> waitStages(waitSemaphoreCount);

        for(size_t i = 0; i < waitSemaphoreCount; ++i) {
            waitSemaphores[i] = polyCast<VKSemaphore>(submitInfo.waitSemaphores[i].first.get())->getSemaphore();
            waitStages[i]     = VKPipelineObject::convertStage(submitInfo.waitSemaphores[i].second);
        }

        //Command buffers
        size_t const commandBufferCount{ std::size(submitInfo.commandBuffers) };
        std::vector<VkCommandBuffer> commandBuffers(commandBufferCount);
        for(size_t i = 0; i < commandBufferCount; ++i) {
            commandBuffers[i] = polyCast<VKGraphicsCommandBuffer>(submitInfo.commandBuffers[i].get())->getCommandBuffer();
        }

        //Signal semaphores
        size_t const signalSemaphoreCount{ std::size(submitInfo.signalSemaphores) };
        std::vector<VkSemaphore> signalSemaphores(signalSemaphoreCount);
        for(size_t i = 0; i < signalSemaphoreCount; ++i) {
            signalSemaphores[i] = polyCast<VKSemaphore>(submitInfo.signalSemaphores[i].get())->getSemaphore();
        }

        VkSubmitInfo vkSubmitInfo{
            .sType                = VK_STRUCTURE_TYPE_SUBMIT_INFO,
            .waitSemaphoreCount   = static_cast<uint32_t>(waitSemaphoreCount),
            .pWaitSemaphores      = std::data(waitSemaphores),
            .pWaitDstStageMask    = std::data(waitStages),
            .commandBufferCount   = static_cast<uint32_t>(commandBufferCount),
            .pCommandBuffers      = std::data(commandBuffers),
            .signalSemaphoreCount = static_cast<uint32_t>(signalSemaphoreCount),
            .pSignalSemaphores    = std::data(signalSemaphores),
        };

        VkFence const vkFence = fence ? polyCast<VKFence const>(fence)->getFence() : VK_NULL_HANDLE;

        if(vkQueueSubmit(queue, 1, &vkSubmitInfo, vkFence) != VK_SUCCESS) {
            GARLIC_LOG(garlicLogContext, garlic::LogLevel::Error, "Failed to submit graphics command buffer(s)");
        }
    }
}