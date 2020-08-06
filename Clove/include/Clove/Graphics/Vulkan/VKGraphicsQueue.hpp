#pragma once

#include "Clove/Graphics/GraphicsQueue.hpp"

namespace clv::gfx::vk {
    class VKGraphicsQueue : public GraphicsQueue {
        //VARIABLES
    private:
        VkDevice device           = VK_NULL_HANDLE;
        VkQueue queue             = VK_NULL_HANDLE;
        VkCommandPool commandPool = VK_NULL_HANDLE;

        //FUNCTIONS
    public:
        VKGraphicsQueue() = delete;
        VKGraphicsQueue(VkDevice device, uint32_t queueFamilyIndex, CommandQueueDescriptor descriptor);

        VKGraphicsQueue(const VKGraphicsQueue& other) = delete;
        VKGraphicsQueue(VKGraphicsQueue&& other) noexcept;

        VKGraphicsQueue& operator=(const VKGraphicsQueue& other) = delete;
        VKGraphicsQueue& operator=(VKGraphicsQueue&& other) noexcept;

        ~VKGraphicsQueue();

        std::unique_ptr<GraphicsCommandBuffer> allocateCommandBuffer() override;
        void freeCommandBuffer(GraphicsCommandBuffer& buffer) override;

        void submit(const GraphicsSubmitInfo& submitInfo, const std::shared_ptr<Fence>& fence) override;
    };
}