#pragma once

#include "Clove/Graphics/GraphicsQueue.hpp"
#include "Clove/Graphics/Vulkan/DevicePointer.hpp"
#include "Clove/Graphics/Vulkan/VulkanTypes.hpp"

#include <vulkan/vulkan.h>

namespace garlic::clove {
    class VKGraphicsQueue : public GraphicsQueue {
        //VARIABLES
    private:
        DevicePointer device;
        VkQueue queue{ VK_NULL_HANDLE };
        VkCommandPool commandPool{ VK_NULL_HANDLE };

        QueueFamilyIndices queueFamilyIndices;

        //FUNCTIONS
    public:
        VKGraphicsQueue() = delete;
        VKGraphicsQueue(DevicePointer device, VkQueue queue, VkCommandPool commandPool, QueueFamilyIndices queueFamilyIndices);

        VKGraphicsQueue(VKGraphicsQueue const &other) = delete;
        VKGraphicsQueue(VKGraphicsQueue &&other) noexcept;

        VKGraphicsQueue &operator=(VKGraphicsQueue const &other) = delete;
        VKGraphicsQueue &operator=(VKGraphicsQueue &&other) noexcept;

        ~VKGraphicsQueue();

        std::unique_ptr<GraphicsCommandBuffer> allocateCommandBuffer() override;
        void freeCommandBuffer(GraphicsCommandBuffer &buffer) override;

        void submit(std::vector<GraphicsSubmitInfo> const &submissions, Fence const *signalFence) override;
    };
}