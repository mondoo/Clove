#pragma once

#include "Clove/Graphics/GraphicsCommandBuffer.hpp"
#include "Clove/Graphics/PipelineObject.hpp"
#include "Clove/Graphics/Queue.hpp"

namespace clv::gfx {
    class Fence;
    class Semaphore;

    struct GraphicsSubmitInfo {
        std::vector<std::pair<std::shared_ptr<Semaphore>, PipelineObject::Stage>> waitSemaphores; /**< Which semaphore to wait on at what stage */
        std::vector<std::shared_ptr<GraphicsCommandBuffer>> commandBuffers;                       /**< The command buffers to execute */
        std::vector<std::shared_ptr<Semaphore>> signalSemaphores;                                 /**< The semaphores that will be signaled when completed */
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
        virtual void freeCommandBuffer(GraphicsCommandBuffer &buffer)          = 0;

        /**
         * @brief Submit command buffers to be processed.
         * @param signalFence An optional fence that will be signaled when this submission is complete.
         */
        virtual void submit(GraphicsSubmitInfo const &submitInfo, Fence const *signalFence) = 0;
    };
}