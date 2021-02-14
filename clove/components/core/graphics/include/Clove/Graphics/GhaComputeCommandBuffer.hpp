#pragma once

#include "Clove/Graphics/CommandBuffer.hpp"
#include "Clove/Graphics/GhaShader.hpp"
#include "Clove/Graphics/MemoryBarrier.hpp"
#include "Clove/Graphics/PipelineObject.hpp"

#include <Clove/Maths/Vector.hpp>

namespace garlic::clove {
    class GhaBuffer;
    class GhaComputePipelineObject;
    class GhaDescriptorSet;
    class GhaImage;
}

namespace garlic::clove {
    /**
     * @brief A buffer for recording compute commands.
     * @details Allocated from a GhaComputeQueue.
     */
    class GhaComputeCommandBuffer {
        //FUNCTIONS
    public:
        virtual ~GhaComputeCommandBuffer() = default;

        virtual void beginRecording(CommandBufferUsage usageFlag) = 0;
        virtual void endRecording()                               = 0;

        virtual void bindPipelineObject(GhaComputePipelineObject &pipelineObject) = 0;

        virtual void bindDescriptorSet(GhaDescriptorSet &descriptorSet, uint32_t const setNum) = 0;
        virtual void pushConstant(size_t const offset, size_t const size, void const *data) = 0;

        virtual void disptach(vec3ui groupCount) = 0;

        /**
         * @brief Creates a memory barrier for a buffer. Allowing for how it's accessed to be changed and/or to transfer queue ownership.
         * @param buffer The buffer to create the barrier for.
         * @param barrierInfo The information about the barrier.
         * @param sourceStage The pipeline stage that gets executed before the barrier.
         * @param destinationStage The pipeline stage executed after the barrier that waits for the results of the sourceStage.
         */
        virtual void bufferMemoryBarrier(GhaBuffer &buffer, BufferMemoryBarrierInfo const &barrierInfo, PipelineStage sourceStage, PipelineStage destinationStage) = 0;
        /**
         * @brief Creates a memory barrier for an image. Allowing for how it's accessed, it's layout and queue ownership to change.
         * @param image The image to create the barrier for.
         * @param barrierInfo The information about the barrier.
         * @param sourceStage The pipeline stage that gets executed before the barrier.
         * @param destinationStage The pipeline stage executed after the barrier that waits for the results of the sourceStage.
         */
        virtual void imageMemoryBarrier(GhaImage &image, ImageMemoryBarrierInfo const &barrierInfo, PipelineStage sourceStage, PipelineStage destinationStage) = 0;
    };
}