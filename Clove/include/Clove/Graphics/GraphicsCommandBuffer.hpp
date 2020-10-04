#pragma once

#include "Clove/Graphics/GraphicsTypes.hpp"
#include "Clove/Graphics/Shader.hpp"

#include <span>

namespace clv::gfx {
    class RenderPass;
    class Framebuffer;
    class PipelineObject;
    class GraphicsBuffer;
    class DescriptorSet;
    class GraphicsImage;
    class GraphicsBuffer;
}

namespace clv::gfx {
    struct RenderArea {
        mth::vec2i origin;
        mth::vec2ui size;
    };

    struct DepthStencilValue {
        float depth{ 0.0f };
        uint32_t stencil{ 0 };
    };

    struct ClearValue {
        mth::vec4f colour;
        DepthStencilValue depthStencil;
    };
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

        /**
         * @details Begins a RenderPass. All subsiquent calls will use this render pass.
         * @param renderPass The RenderPass to begin.
         * @param frameBuffer The FrameBuffer to use.
         * @param renderArea What area to render onto the frameBuffer.
         * @param clearValues An array of clear values. Each element in the array represents an attachment in the frameBuffer.
         */
        virtual void beginRenderPass(RenderPass& renderPass, Framebuffer& frameBuffer, const RenderArea& renderArea, std::span<ClearValue> clearValues) = 0;
        virtual void endRenderPass()                                                                                                                    = 0;

        virtual void bindPipelineObject(PipelineObject& pipelineObject)                                                     = 0;
        virtual void bindVertexBuffer(GraphicsBuffer& vertexBuffer, const uint32_t binding)                                 = 0;
        virtual void bindIndexBuffer(GraphicsBuffer& indexBuffer, IndexType indexType)                                      = 0;
        virtual void bindDescriptorSet(DescriptorSet& descriptorSet, const PipelineObject& pipeline, const uint32_t setNum) = 0;

        virtual void pushConstant(PipelineObject& pipelineObject, const Shader::Stage stage, const uint32_t size, const void* data) = 0;

        virtual void drawIndexed(const size_t indexCount) = 0;

        virtual void bufferMemoryBarrier(GraphicsBuffer& buffer, const BufferMemoryBarrierInfo& barrierInfo, PipelineStage sourceStage, PipelineStage destinationStage) = 0;
        virtual void imageMemoryBarrier(GraphicsImage& image, const ImageMemoryBarrierInfo& barrierInfo, PipelineStage sourceStage, PipelineStage destinationStage)     = 0;
    };
}