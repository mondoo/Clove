#pragma once

#include "Clove/Graphics/GraphicsTypes.hpp"

namespace clv::gfx {
    class RenderPass;
    class Framebuffer;
    class PipelineObject;
    class GraphicsBuffer;
    class DescriptorSet;
    class GraphicsImage;
}

namespace clv::gfx {
    /**
     * @brief A buffer for recording graphics commands.
     * @details Allocated from a GraphicsQueue.
     */
    class GraphicsCommandBuffer {
        //FUNCTIONS
    public:
        virtual ~GraphicsCommandBuffer() = default;

        virtual void beginRecording(CommandBufferUsage usageFlag) = 0;
        virtual void endRecording()                               = 0;

        virtual void beginRenderPass(RenderPass& renderPass, Framebuffer& frameBuffer, const RenderArea& renderArea, const mth::vec4f& clearColour, const DepthStencilValue& depthStencilClearValue) = 0;
        virtual void endRenderPass()                                                                                                                                                                 = 0;

        virtual void bindPipelineObject(PipelineObject& pipelineObject)                              = 0;
        virtual void bindVertexBuffer(GraphicsBuffer& vertexBuffer)                                  = 0;
        virtual void bindIndexBuffer(GraphicsBuffer& indexBuffer, IndexType indexType)               = 0;
        virtual void bindDescriptorSet(DescriptorSet& descriptorSet, const PipelineObject& pipeline) = 0;

        virtual void drawIndexed(const size_t indexCount) = 0;

        virtual void transitionImageLayout(GraphicsImage& image, ImageLayout previousLayout, ImageLayout newLayout) = 0;
    };
}