#pragma once

namespace clv::gfx {
    class GraphicsBuffer;
    class GraphicsImage;
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

        virtual void copyBufferToBuffer(GraphicsBuffer& source, const size_t sourceOffset, GraphicsBuffer& destination, const size_t destinationOffset, const size_t sizeBytes)                                                 = 0;
        virtual void copyBufferToImage(GraphicsBuffer& source, const size_t sourceOffset, GraphicsImage& destination, ImageLayout destinationLayout, const mth::vec3i& destinationOffset, const mth::vec3ui& destinationExtent) = 0;

        virtual void bufferMemoryBarrier(GraphicsBuffer& buffer, const BufferMemoryBarrierInfo& barrierInfo, PipelineStage sourceStage, PipelineStage destinationStage) = 0;
        virtual void imageMemoryBarrier(GraphicsImage& image, const ImageMemoryBarrierInfo& barrierInfo, PipelineStage sourceStage, PipelineStage destinationStage)     = 0;
    };
}