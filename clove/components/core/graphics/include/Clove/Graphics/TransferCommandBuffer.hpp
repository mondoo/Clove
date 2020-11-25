#pragma once

#include "Clove/Graphics/CommandBuffer.hpp"
#include "Clove/Graphics/GraphicsImage.hpp"
#include "Clove/Graphics/MemoryBarrier.hpp"
#include "Clove/Graphics/PipelineObject.hpp"

#include <Clove/Maths/Vector.hpp>

namespace garlic::clove {
    class GraphicsBuffer;
}

namespace garlic::clove {
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
        virtual void copyBufferToImage(GraphicsBuffer &source, size_t const sourceOffset, GraphicsImage &destination, vec3i const &destinationOffset, vec3ui const &destinationExtent) = 0;

        virtual void copyImageToBuffer(GraphicsImage &source, vec3i const &sourceOffset, vec3ui const &sourceExtent, GraphicsBuffer &destination, size_t const destinationOffset) = 0;

        virtual void bufferMemoryBarrier(GraphicsBuffer &buffer, BufferMemoryBarrierInfo const &barrierInfo, PipelineObject::Stage sourceStage, PipelineObject::Stage destinationStage) = 0;
        virtual void imageMemoryBarrier(GraphicsImage &image, ImageMemoryBarrierInfo const &barrierInfo, PipelineObject::Stage sourceStage, PipelineObject::Stage destinationStage)     = 0;
    };
}