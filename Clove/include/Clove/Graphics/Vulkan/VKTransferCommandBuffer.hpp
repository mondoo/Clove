#pragma once

#include "Clove/Graphics/GraphicsTypes.hpp"
#include "Clove/Graphics/TransferCommandBuffer.hpp"

#include <vulkan/vulkan.h>

namespace clv::gfx::vk {
    class VKTransferCommandBuffer : public TransferCommandBuffer {
        //VARIABLES
    private:
        VkCommandBuffer commandBuffer = VK_NULL_HANDLE;

        //FUNCTIONS
    public:
        VKTransferCommandBuffer() = delete;
        VKTransferCommandBuffer(VkCommandBuffer commandBuffer);

        VKTransferCommandBuffer(const VKTransferCommandBuffer& other) = delete;
        VKTransferCommandBuffer(VKTransferCommandBuffer&& other) noexcept;

        VKTransferCommandBuffer& operator=(const VKTransferCommandBuffer& other) = delete;
        VKTransferCommandBuffer& operator=(VKTransferCommandBuffer&& other) noexcept;

        ~VKTransferCommandBuffer();

        void beginRecording(CommandBufferUsage usageFlag) override;
        void endRecording() override;

        void copyBufferToBuffer(GraphicsBuffer& source, const size_t sourceOffset, GraphicsBuffer& destination, const size_t destinationOffset, const size_t sizeBytes) override;
        void copyBufferToImage(GraphicsBuffer& source, const size_t sourceOffset, GraphicsImage& destination, ImageLayout destinationLayout, const mth::vec3i& destinationOffset, const mth::vec3ui& destinationExtent) override;

        void transitionImageLayout(GraphicsImage& image, ImageLayout previousLayout, ImageLayout newLayout) override;

        VkCommandBuffer getCommandBuffer() const;
    };
}