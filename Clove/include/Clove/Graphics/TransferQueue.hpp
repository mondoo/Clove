#pragma once

#include "Clove/Graphics/Queue.hpp"
#include "Clove/Graphics/TransferCommandBuffer.hpp"

namespace clv::gfx {
    class Fence;
    class Semaphore;

    struct TransferSubmitInfo {
        std::vector<std::pair<std::shared_ptr<Semaphore>, PipelineObject::Stage>> waitSemaphores; /**< What semaphores to wait on at what stage */
        std::vector<std::shared_ptr<TransferCommandBuffer>> commandBuffers;                       /**< The command buffers to execute */
        std::vector<std::shared_ptr<Semaphore>> signalSemaphores;                                 /**< The semaphores that will be signaled when completed */
    };
}

namespace clv::gfx {
    /**
     * @brief Creates buffers that can record transfer commands and then be submitted.
     */
    class TransferQueue {
        //FUNCTIONS
    public:
        virtual ~TransferQueue() = default;

        virtual std::unique_ptr<TransferCommandBuffer> allocateCommandBuffer() = 0;
        virtual void freeCommandBuffer(TransferCommandBuffer &buffer)          = 0;

        /**
         * @brief Submit command buffers to be processed.
         * @param signalFence An optional fence that will be signaled when all submissions are complete.
         */
        virtual void submit(std::vector<TransferSubmitInfo> const &submissions, Fence const *signalFence) = 0;
    };
}