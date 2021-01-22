#include "Clove/Graphics/Vulkan/VKGraphicsQueue.hpp"

#include "Clove/Graphics/Vulkan/VKFence.hpp"
#include "Clove/Graphics/Vulkan/VKGraphicsCommandBuffer.hpp"
#include "Clove/Graphics/Vulkan/VKPipelineObject.hpp"
#include "Clove/Graphics/Vulkan/VKSemaphore.hpp"

#include <Clove/Cast.hpp>
#include <Clove/Definitions.hpp>
#include <Clove/Log/Log.hpp>

namespace garlic::clove {
    VKGraphicsQueue::VKGraphicsQueue(DevicePointer device, VkQueue queue, VkCommandPool commandPool, QueueFamilyIndices queueFamilyIndices)
        : device{ std::move(device) }
        , queue{ queue }
        , commandPool{ commandPool }
        , queueFamilyIndices{ std::move(queueFamilyIndices) } {
    }

    VKGraphicsQueue::VKGraphicsQueue(VKGraphicsQueue &&other) noexcept = default;

    VKGraphicsQueue &VKGraphicsQueue::operator=(VKGraphicsQueue &&other) noexcept = default;

    VKGraphicsQueue::~VKGraphicsQueue() {
        vkDestroyCommandPool(device.get(), commandPool, nullptr);
    }

    std::unique_ptr<GhaGraphicsCommandBuffer> VKGraphicsQueue::allocateCommandBuffer() {
        VkCommandBuffer commandBuffer;

        VkCommandBufferAllocateInfo allocInfo{
            .sType              = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO,
            .commandPool        = commandPool,
            .level              = VK_COMMAND_BUFFER_LEVEL_PRIMARY,
            .commandBufferCount = 1,
        };

        if(vkAllocateCommandBuffers(device.get(), &allocInfo, &commandBuffer) != VK_SUCCESS) {
            CLOVE_LOG(LOG_CATEGORY_CLOVE, LogLevel::Error, "Failed to allocate command buffer");
            return nullptr;
        }

        return std::make_unique<VKGraphicsCommandBuffer>(commandBuffer, queueFamilyIndices);
    }

    void VKGraphicsQueue::freeCommandBuffer(GhaGraphicsCommandBuffer &buffer) {
        VkCommandBuffer buffers[] = { polyCast<VKGraphicsCommandBuffer>(&buffer)->getCommandBuffer() };
        vkFreeCommandBuffers(device.get(), commandPool, 1, buffers);
    }

    void VKGraphicsQueue::submit(std::vector<GraphicsSubmitInfo> const &submissions, GhaFence const *signalFence) {
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
                commandBuffers[i][j] = polyCast<VKGraphicsCommandBuffer>(submissions[i].commandBuffers[j].get())->getCommandBuffer();
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
            CLOVE_LOG(LOG_CATEGORY_CLOVE, LogLevel::Error, "Failed to submit graphics command buffer(s)");
        }
    }
}