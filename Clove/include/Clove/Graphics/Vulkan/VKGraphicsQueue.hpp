#pragma once

#include "Clove/Graphics/Vulkan/VKGraphicsCommandBuffer.hpp"

#include "Clove/Graphics/Vulkan/VKSemaphore.hpp"
#include "Clove/Graphics/Vulkan/VKFence.hpp"

namespace clv::gfx{
    struct GraphicsSubmitInfo {
        //Each element in the semaphore maps to an element in the waitStages
        std::vector<std::shared_ptr<vk::VKSemaphore>> waitSemaphores;
        std::vector<WaitStage> waitStages;

        std::vector<std::shared_ptr<vk::VKGraphicsCommandBuffer>> commandBuffers;

        std::vector<std::shared_ptr<vk::VKSemaphore>> signalSemaphores;
    };
}

namespace clv::gfx::vk{
    //Creates buffers for encoding graphics commands
    class VKGraphicsQueue {
        //VARIABLES
    private:
        //uint32_t queueFamilyIndex = 0;
        VkDevice device           = VK_NULL_HANDLE;
        VkQueue queue             = VK_NULL_HANDLE;
        VkCommandPool commandPool = VK_NULL_HANDLE;

        //FUNCTIONS
    public:
        //TODO: Ctors
        VKGraphicsQueue() = delete;
        VKGraphicsQueue(VkDevice device, uint32_t queueFamilyIndex, CommandQueueDescriptor descriptor);

        ~VKGraphicsQueue();

        std::unique_ptr<VKGraphicsCommandBuffer> allocateCommandBuffer();
        void freeCommandBuffer(VKGraphicsCommandBuffer& buffer);

        void submit(const GraphicsSubmitInfo& submitInfo, const std::shared_ptr<VKFence>& fence);
    };
}