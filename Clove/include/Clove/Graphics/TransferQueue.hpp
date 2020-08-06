#pragma once

#include "Clove/Graphics/TransferCommandBuffer.hpp"

namespace clv::gfx {
    struct TransferSubmitInfo {
        std::vector<std::shared_ptr<TransferCommandBuffer>> commandBuffers;
    };
}

namespace clv::gfx {
    /**
     * Creates buffers that can record transfer commands and then be submitted.
     */
    class TransferQueue {
        //FUNCTIONS
    public:
        virtual ~TransferQueue() = default;

        virtual std::unique_ptr<TransferCommandBuffer> allocateCommandBuffer() = 0;
        virtual void freeCommandBuffer(TransferCommandBuffer& buffer)          = 0;

        virtual void submit(const TransferSubmitInfo& submitInfo) = 0;
    };
}