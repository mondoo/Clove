#include "Clove/Graphics/Vulkan/VKGraphicsQueue.hpp"

#include "Clove/Graphics/Vulkan/VKFence.hpp"
#include "Clove/Graphics/Vulkan/VKGraphicsCommandBuffer.hpp"
#include "Clove/Graphics/Vulkan/VKPipelineObject.hpp"
#include "Clove/Graphics/Vulkan/VKQueue.hpp"
#include "Clove/Graphics/Vulkan/VKSemaphore.hpp"
#include "Clove/Log.hpp"
#include "Clove/Utils/Cast.hpp"

#include <Root/Definitions.hpp>

namespace clv::gfx::vk {
    VKGraphicsQueue::VKGraphicsQueue(DevicePointer device, QueueFamilyIndices queueFamilyIndices, CommandQueueDescriptor descriptor)
        : device(std::move(device))
        , queueFamilyIndices(std::move(queueFamilyIndices)) {
        const uint32_t familyIndex = *this->queueFamilyIndices.graphicsFamily;

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

    VKGraphicsQueue::VKGraphicsQueue(VKGraphicsQueue&& other) noexcept = default;

    VKGraphicsQueue& VKGraphicsQueue::operator=(VKGraphicsQueue&& other) noexcept = default;

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
            GARLIC_LOG(garlicLogContext, Log::Level::Error, "Failed to allocate command buffer");
            return nullptr;
        }

        return std::make_unique<VKGraphicsCommandBuffer>(commandBuffer, queueFamilyIndices);
    }

    void VKGraphicsQueue::freeCommandBuffer(GraphicsCommandBuffer& buffer) {
        VkCommandBuffer buffers[] = { polyCast<VKGraphicsCommandBuffer>(&buffer)->getCommandBuffer() };
        vkFreeCommandBuffers(device.get(), commandPool, 1, buffers);
    }

    void VKGraphicsQueue::submit(const GraphicsSubmitInfo& submitInfo, const Fence* fence) {
        //Wait semaphores / stages
        const size_t waitSemaphoreCount = std::size(submitInfo.waitSemaphores);
        std::vector<VkSemaphore> waitSemaphores(waitSemaphoreCount);
        for(size_t i = 0; i < waitSemaphoreCount; ++i) {
            waitSemaphores[i] = polyCast<VKSemaphore>(submitInfo.waitSemaphores[i].get())->getSemaphore();
        }
        std::vector<VkPipelineStageFlags> waitStages(waitSemaphoreCount);
        for(size_t i = 0; i < waitSemaphoreCount; ++i) {
            waitStages[i] = VKPipelineObject::convertStage(submitInfo.waitStages[i]);
        }

        //Command buffers
        const size_t commandBufferCount = std::size(submitInfo.commandBuffers);
        std::vector<VkCommandBuffer> commandBuffers(commandBufferCount);
        for(size_t i = 0; i < commandBufferCount; ++i) {
            commandBuffers[i] = polyCast<VKGraphicsCommandBuffer>(submitInfo.commandBuffers[i].get())->getCommandBuffer();
        }

        //Signal semaphores
        const size_t signalSemaphoreCount = std::size(submitInfo.signalSemaphores);
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

        if(vkQueueSubmit(queue, 1, &vkSubmitInfo, fence ? polyCast<const VKFence>(fence)->getFence() : VK_NULL_HANDLE) != VK_SUCCESS) {
            GARLIC_LOG(garlicLogContext, Log::Level::Error, "Failed to submit graphics command buffer(s)");
        }
    }
}