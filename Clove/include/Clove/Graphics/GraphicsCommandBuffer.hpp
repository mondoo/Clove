#pragma once

#include "Clove/Graphics/CommandBuffer.hpp"
#include "Clove/Graphics/MemoryBarrier.hpp"
#include "Clove/Graphics/PipelineObject.hpp"
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

    enum class IndexType {
        Uint16
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
        virtual void beginRenderPass(RenderPass &renderPass, Framebuffer &frameBuffer, RenderArea const &renderArea, std::span<ClearValue> clearValues) = 0;
        virtual void endRenderPass()                                                                                                                    = 0;

        virtual void bindPipelineObject(PipelineObject &pipelineObject) = 0;
        /**
         * @brief Bind a vertex buffer to be used in the next draw call.
         * @param offset Offset into the buffer where the vertices begin.
         */
        virtual void bindVertexBuffer(GraphicsBuffer &vertexBuffer, size_t const offset) = 0;
        /**
         * @brief Bind an index buffer to be used in the next draw call.
         * @param offset Offset into the buffer where the indices being.
         */
        virtual void bindIndexBuffer(GraphicsBuffer &indexBuffer, size_t const offset, IndexType indexType) = 0;
        virtual void bindDescriptorSet(DescriptorSet &descriptorSet, uint32_t const setNum)                 = 0;

        virtual void pushConstant(Shader::Stage const stage, size_t const offset, size_t const size, void const *data) = 0;

        virtual void drawIndexed(size_t const indexCount) = 0;

        virtual void bufferMemoryBarrier(GraphicsBuffer &buffer, BufferMemoryBarrierInfo const &barrierInfo, PipelineObject::Stage sourceStage, PipelineObject::Stage destinationStage) = 0;
        virtual void imageMemoryBarrier(GraphicsImage &image, ImageMemoryBarrierInfo const &barrierInfo, PipelineObject::Stage sourceStage, PipelineObject::Stage destinationStage)     = 0;
    };
}