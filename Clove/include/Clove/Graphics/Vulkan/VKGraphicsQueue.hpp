#pragma once

#include "Clove/Graphics/GraphicsQueue.hpp"
#include "Clove/Graphics/Vulkan/VulkanTypes.hpp"
#include "Clove/Graphics/Vulkan/DevicePointer.hpp"

#include <vulkan/vulkan.h>

namespace clv::gfx::vk {
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
        VKGraphicsQueue(DevicePointer device, QueueFamilyIndices queueFamilyIndices, CommandQueueDescriptor descriptor);

        VKGraphicsQueue(const VKGraphicsQueue& other) = delete;
        VKGraphicsQueue(VKGraphicsQueue&& other) noexcept;

        VKGraphicsQueue& operator=(const VKGraphicsQueue& other) = delete;
        VKGraphicsQueue& operator=(VKGraphicsQueue&& other) noexcept;

        ~VKGraphicsQueue();

        std::unique_ptr<GraphicsCommandBuffer> allocateCommandBuffer() override;
        void freeCommandBuffer(GraphicsCommandBuffer& buffer) override;

        void submit(const GraphicsSubmitInfo& submitInfo, const Fence* fence) override;
    };
}