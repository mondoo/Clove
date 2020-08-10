#include "Clove/Graphics/Vulkan/VKGraphicsQueue.hpp"

#include "Clove/Graphics/Vulkan/VKSemaphore.hpp"
#include "Clove/Graphics/Vulkan/VKFence.hpp"
#include "Clove/Graphics/Vulkan/VKGraphicsCommandBuffer.hpp"

namespace clv::gfx::vk {
    static VkPipelineStageFlagBits getPipelineStageFlag(WaitStage stage) {
        switch(stage) {
            case WaitStage::ColourAttachmentOutput:
                return VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
        }
    }

    VKGraphicsQueue::VKGraphicsQueue(VkDevice device, uint32_t queueFamilyIndex, CommandQueueDescriptor descriptor)
        : device(device) {
        vkGetDeviceQueue(device, queueFamilyIndex, 0, &queue);

        VkCommandPoolCreateInfo poolInfo{};
        poolInfo.sType            = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
        poolInfo.pNext            = nullptr;
        poolInfo.flags            = descriptor.flags == QueueFlags::Transient ? VK_COMMAND_POOL_CREATE_TRANSIENT_BIT : 0;
        poolInfo.queueFamilyIndex = queueFamilyIndex;

        if(vkCreateCommandPool(device, &poolInfo, nullptr, &commandPool) != VK_SUCCESS) {
            GARLIC_LOG(garlicLogContext, Log::Level::Error, "Failed to create graphics command pool");
        }
    }

    VKGraphicsQueue::VKGraphicsQueue(VKGraphicsQueue&& other) noexcept = default;

    VKGraphicsQueue& VKGraphicsQueue::operator=(VKGraphicsQueue&& other) noexcept = default;

    VKGraphicsQueue::~VKGraphicsQueue() {
        vkDestroyCommandPool(device, commandPool, nullptr);
    }

    std::unique_ptr<GraphicsCommandBuffer> VKGraphicsQueue::allocateCommandBuffer() {
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

        return std::make_unique<VKGraphicsCommandBuffer>(commandBuffer);
    }

    void VKGraphicsQueue::freeCommandBuffer(GraphicsCommandBuffer& buffer) {
        VkCommandBuffer buffers[] = { polyCast<VKGraphicsCommandBuffer>(&buffer)->getCommandBuffer() };
        vkFreeCommandBuffers(device, commandPool, 1, buffers);
    }

    void VKGraphicsQueue::submit(const GraphicsSubmitInfo& submitInfo, const std::shared_ptr<Fence>& fence) {
        //Wait semaphores / stages
        const size_t waitSemaphoreCount = std::size(submitInfo.waitSemaphores);
        std::vector<VkSemaphore> waitSemaphores(waitSemaphoreCount);
        for(size_t i = 0; i < waitSemaphoreCount; ++i) {
            waitSemaphores[i] = polyCast<VKSemaphore>(submitInfo.waitSemaphores[i].get())->getSemaphore();
        }
        std::vector<VkPipelineStageFlags> waitStages(waitSemaphoreCount);
        for(size_t i = 0; i < waitSemaphoreCount; ++i) {
            waitStages[i] = getPipelineStageFlag(submitInfo.waitStages[i]);
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

        VkSubmitInfo vkSubmitInfo{};
        vkSubmitInfo.sType                = VK_STRUCTURE_TYPE_SUBMIT_INFO;
        vkSubmitInfo.waitSemaphoreCount   = waitSemaphoreCount;
        vkSubmitInfo.pWaitSemaphores      = std::data(waitSemaphores);
        vkSubmitInfo.pWaitDstStageMask    = std::data(waitStages);
        vkSubmitInfo.commandBufferCount   = commandBufferCount;
        vkSubmitInfo.pCommandBuffers      = std::data(commandBuffers);
        vkSubmitInfo.signalSemaphoreCount = signalSemaphoreCount;
        vkSubmitInfo.pSignalSemaphores    = std::data(signalSemaphores);

        if(vkQueueSubmit(queue, 1, &vkSubmitInfo, fence ? polyCast<VKFence>(fence.get())->getFence() : VK_NULL_HANDLE) != VK_SUCCESS) {
            GARLIC_LOG(garlicLogContext, Log::Level::Error, "Failed to submit graphics command buffer(s)");
        }
    }
}