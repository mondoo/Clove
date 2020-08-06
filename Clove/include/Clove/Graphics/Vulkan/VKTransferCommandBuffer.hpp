#pragma once

#include "Clove/Graphics/GraphicsTypes.hpp"

#include <vulkan/vulkan.h>

//TODO: remove
#include "Clove/Graphics/Vulkan/VKBuffer.hpp"
#include "Clove/Graphics/Vulkan/VKDescriptorSet.hpp"
#include "Clove/Graphics/Vulkan/VKFramebuffer.hpp"
#include "Clove/Graphics/Vulkan/VKImage.hpp"
#include "Clove/Graphics/Vulkan/VKPipelineObject.hpp"
#include "Clove/Graphics/Vulkan/VKRenderPass.hpp"

namespace clv::gfx::vk {
    class VKTransferCommandBuffer {
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

        void beginRecording(CommandBufferUsage usageFlag);
        void endRecording();

        void copyBufferToBuffer(VKBuffer& source, const size_t sourceOffset, VKBuffer& destination, const size_t destinationOffset, const size_t sizeBytes);
        void copyBufferToImage(VKBuffer& source, const size_t sourceOffset, VKImage& destination, ImageLayout destinationLayout, const mth::vec3i& destinationOffset, const mth::vec3ui& destinationExtent);

        void transitionImageLayout(VKImage& image, ImageLayout previousLayout, ImageLayout newLayout);

        VkCommandBuffer getCommandBuffer() const;
    };
}