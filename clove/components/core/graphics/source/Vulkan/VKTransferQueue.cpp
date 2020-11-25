#include "Clove/Graphics/Vulkan/VKTransferQueue.hpp"

#include "Clove/Graphics/Vulkan/VKFence.hpp"
#include "Clove/Graphics/Vulkan/VKPipelineObject.hpp"
#include "Clove/Graphics/Vulkan/VKQueue.hpp"
#include "Clove/Graphics/Vulkan/VKSemaphore.hpp"
#include "Clove/Graphics/Vulkan/VKTransferCommandBuffer.hpp"

#include <Clove/Cast.hpp>
#include <Clove/Definitions.hpp>
#include <Clove/Log/Log.hpp>

namespace garlic::clove {
    VKTransferQueue::VKTransferQueue(DevicePointer device, QueueFamilyIndices queueFamilyIndices, CommandQueueDescriptor descriptor)
        : device(std::move(device))
        , queueFamilyIndices(std::move(queueFamilyIndices)) {
        uint32_t const familyIndex = *this->queueFamilyIndices.transferFamily;

        vkGetDeviceQueue(this->device.get(), familyIndex, 0, &queue);

        VkCommandPoolCreateInfo poolInfo{
            .sType            = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO,
            .pNext            = nullptr,
            .flags            = convertCommandPoolCreateFlags(descriptor.flags),
            .queueFamilyIndex = familyIndex,
        };

        if(vkCreateCommandPool(this->device.get(), &poolInfo, nullptr, &commandPool) != VK_SUCCESS) {
            GARLIC_LOG(LOG_CATEGORY_CLOVE, LogLevel::Error, "Failed to create graphics command pool");
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
            GARLIC_LOG(LOG_CATEGORY_CLOVE, LogLevel::Error, "Failed to allocate command buffer");
            return nullptr;
        }

        return std::make_unique<VKTransferCommandBuffer>(commandBuffer, queueFamilyIndices);
    }

    void VKTransferQueue::freeCommandBuffer(TransferCommandBuffer &buffer) {
        VkCommandBuffer buffers[] = { polyCast<VKTransferCommandBuffer>(&buffer)->getCommandBuffer() };
        vkFreeCommandBuffers(device.get(), commandPool, 1, buffers);
    }

    void VKTransferQueue::submit(std::vector<TransferSubmitInfo> const &submissions, Fence const *signalFence) {
        auto const submissioncount{ std::size(submissions) };
        std::vector<VkSubmitInfo> vkSubmissions;
        vkSubmissions.reserve(submissioncount);

        std::vector<std::vector<VkSemaphore>> waitSemaphores(submissioncount);
        std::vector<std::vector<VkPipelineStageFlags>> waitStages(submissioncount);
        std::vector<std::vector<VkCommandBuffer>> commandBuffers(submissioncount);
        std::vector<std::vector<VkSemaphore>> signalSemaphores(submissioncount);

        for(size_t i{ 0 }; i < submissioncount; ++i) {
            //Wait semaphores / stages
            size_t const waitSemaphoreCount{ std::size(submissions[i].waitSemaphores) };
            waitSemaphores[i].resize(waitSemaphoreCount);
            waitStages[i].resize(waitSemaphoreCount);

            for(size_t j = 0; j < waitSemaphoreCount; ++j) {
                waitSemaphores[i][j] = polyCast<VKSemaphore>(submissions[i].waitSemaphores[j].first.get())->getSemaphore();
                waitStages[i][j]     = VKPipelineObject::convertStage(submissions[i].waitSemaphores[j].second);
            }

            //Command buffers
            size_t const commandBufferCount{ std::size(submissions[i].commandBuffers) };
            commandBuffers[i].resize(commandBufferCount);
            for(size_t j = 0; j < commandBufferCount; ++j) {
                commandBuffers[i][j] = polyCast<VKTransferCommandBuffer>(submissions[i].commandBuffers[j].get())->getCommandBuffer();
            }

            //Signal semaphores
            size_t const signalSemaphoreCount{ std::size(submissions[i].signalSemaphores) };
            signalSemaphores[i].resize(signalSemaphoreCount);
            for(size_t j = 0; j < signalSemaphoreCount; ++j) {
                signalSemaphores[i][j] = polyCast<VKSemaphore>(submissions[i].signalSemaphores[j].get())->getSemaphore();
            }

            vkSubmissions.emplace_back(VkSubmitInfo{
                .sType                = VK_STRUCTURE_TYPE_SUBMIT_INFO,
                .waitSemaphoreCount   = static_cast<uint32_t>(waitSemaphoreCount),
                .pWaitSemaphores      = std::data(waitSemaphores[i]),
                .pWaitDstStageMask    = std::data(waitStages[i]),
                .commandBufferCount   = static_cast<uint32_t>(commandBufferCount),
                .pCommandBuffers      = std::data(commandBuffers[i]),
                .signalSemaphoreCount = static_cast<uint32_t>(signalSemaphoreCount),
                .pSignalSemaphores    = std::data(signalSemaphores[i]),
            });
        }

        VkFence const vkFence{ signalFence ? polyCast<VKFence const>(signalFence)->getFence() : VK_NULL_HANDLE };

        if(vkQueueSubmit(queue, std::size(vkSubmissions), std::data(vkSubmissions), vkFence) != VK_SUCCESS) {
            GARLIC_LOG(LOG_CATEGORY_CLOVE, LogLevel::Error, "Failed to submit graphics command buffer(s)");
        }
    }
}