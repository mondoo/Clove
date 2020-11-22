#pragma once

#include "Clove/Graphics/CommandBuffer.hpp"
#include "Clove/Graphics/GraphicsImage.hpp"
#include "Clove/Graphics/MemoryBarrier.hpp"
#include "Clove/Graphics/PipelineObject.hpp"

namespace clv::gfx {
    class GraphicsBuffer;
}

namespace clv::gfx {
    /**
     * @brief A buffer for recording transfer commands.
     * @details Allocated from a TransferQueue.
     */
    class TransferCommandBuffer {
        //FUNCTIONS
    public:
        virtual ~TransferCommandBuffer() = default;

        virtual void beginRecording(CommandBufferUsage usageFlag) = 0;
        virtual void endRecording()                               = 0;

        virtual void copyBufferToBuffer(GraphicsBuffer &source, size_t const sourceOffset, GraphicsBuffer &destination, size_t const destinationOffset, size_t const sizeBytes)                  = 0;
        virtual void copyBufferToImage(GraphicsBuffer &source, size_t const sourceOffset, GraphicsImage &destination, mth::vec3i const &destinationOffset, mth::vec3ui const &destinationExtent) = 0;

        virtual void copyImageToBuffer(GraphicsImage &source, mth::vec3i const &sourceOffset, mth::vec3ui const &sourceExtent, GraphicsBuffer &destination, size_t const destinationOffset) = 0;

        virtual void bufferMemoryBarrier(GraphicsBuffer &buffer, BufferMemoryBarrierInfo const &barrierInfo, PipelineObject::Stage sourceStage, PipelineObject::Stage destinationStage) = 0;
        virtual void imageMemoryBarrier(GraphicsImage &image, ImageMemoryBarrierInfo const &barrierInfo, PipelineObject::Stage sourceStage, PipelineObject::Stage destinationStage)     = 0;
    };
}