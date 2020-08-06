#pragma once

#include "Clove/Graphics/GraphicsTypes.hpp"

#include <vulkan/vulkan.h>

//TODO: Remove
#include "Clove/Graphics/Vulkan/VKBuffer.hpp"
#include "Clove/Graphics/Vulkan/VKDescriptorSet.hpp"
#include "Clove/Graphics/Vulkan/VKFramebuffer.hpp"
#include "Clove/Graphics/Vulkan/VKImage.hpp"
#include "Clove/Graphics/Vulkan/VKPipelineObject.hpp"
#include "Clove/Graphics/Vulkan/VKRenderPass.hpp"

namespace clv::gfx::vk {
    class VKGraphicsCommandBuffer {
        //VARIABLES
    private:
        VkCommandBuffer commandBuffer = VK_NULL_HANDLE;

        //FUNCTIONS
    public:
        VKGraphicsCommandBuffer() = delete;
        VKGraphicsCommandBuffer(VkCommandBuffer commandBuffer);

        VKGraphicsCommandBuffer(const VKGraphicsCommandBuffer& other) = delete;
        VKGraphicsCommandBuffer(VKGraphicsCommandBuffer&& other) noexcept;

        VKGraphicsCommandBuffer& operator=(const VKGraphicsCommandBuffer& other) = delete;
        VKGraphicsCommandBuffer& operator=(VKGraphicsCommandBuffer&& other) noexcept;

        ~VKGraphicsCommandBuffer();

        void beginRecording(CommandBufferUsage usageFlag);
        void endRecording();

        void beginRenderPass(VKRenderPass& renderPass, VKFramebuffer& frameBuffer, const RenderArea& renderArea, const mth::vec4f& clearColour, const DepthStencilValue& depthStencilClearValue);
        void endRenderPass();

        void bindPipelineObject(VKPipelineObject& pipelineObject);
        void bindVertexBuffer(VKBuffer& vertexBuffer);
        void bindIndexBuffer(VKBuffer& indexBuffer, IndexType indexType);
        void bindDescriptorSet(VKDescriptorSet& descriptorSet, const VKPipelineObject& pipeline);

        void drawIndexed(const size_t indexCount);

        void transitionImageLayout(VKImage& image, ImageLayout previousLayout, ImageLayout newLayout);

        VkCommandBuffer getCommandBuffer() const;
    };
}