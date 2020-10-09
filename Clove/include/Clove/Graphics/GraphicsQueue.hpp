#pragma once

#include "Clove/Graphics/GraphicsCommandBuffer.hpp"
#include "Clove/Graphics/PipelineObject.hpp"
#include "Clove/Graphics/Queue.hpp"

namespace clv::gfx {
    class Fence;
    class Semaphore;

    struct GraphicsSubmitInfo {
        std::vector<std::shared_ptr<Semaphore>> waitSemaphores; /**< Each element in the semaphore maps to an element in the waitStages */
        std::vector<PipelineObject::Stage> waitStages;          /**< What stage in the pipeline to wait on the semaphore */
        //TODO: Make the above a pair

        std::vector<std::shared_ptr<GraphicsCommandBuffer>> commandBuffers;

        std::vector<std::shared_ptr<Semaphore>> signalSemaphores;
    };
}

namespace clv::gfx {
    /**
     * @brief Creates buffers that can record graphics commands and then be submitted for rendering.
     */
    class GraphicsQueue {
        //FUNCTIONS
    public:
        virtual ~GraphicsQueue() = default;

        virtual std::unique_ptr<GraphicsCommandBuffer> allocateCommandBuffer() = 0;
        virtual void freeCommandBuffer(GraphicsCommandBuffer& buffer)          = 0;

        virtual void submit(const GraphicsSubmitInfo& submitInfo, const Fence* fence) = 0;
    };
}