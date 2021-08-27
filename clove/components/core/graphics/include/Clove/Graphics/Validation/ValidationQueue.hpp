#pragma once

#include "Clove/Graphics/GhaGraphicsQueue.hpp"
#include "Clove/Graphics/GhaComputeQueue.hpp"
#include "Clove/Graphics/GhaTransferQueue.hpp"

namespace clove {
    template<typename BaseQueueType>
    class ValidationGraphicsQueue : public BaseQueueType {
        //FUNCTIONS
    public:
        using BaseQueueType::BaseQueueType;

        std::unique_ptr<GhaGraphicsCommandBuffer> allocateCommandBuffer() override;

        void submit(std::vector<GraphicsSubmitInfo> const &submissions, GhaFence *signalFence) override;
    };

    template<typename BaseQueueType>
    class ValidationComputeQueue : public BaseQueueType {
        //FUNCTIONS
    public:
        using BaseQueueType::BaseQueueType;

        std::unique_ptr<GhaComputeCommandBuffer> allocateCommandBuffer() override;

        void submit(std::vector<ComputeSubmitInfo> const &submissions, GhaFence *signalFence) override;
    };

    template<typename BaseQueueType>
    class ValidationTransferQueue : public BaseQueueType {
        //FUNCTIONS
    public:
        using BaseQueueType::BaseQueueType;

        std::unique_ptr<GhaTransferCommandBuffer> allocateCommandBuffer() override;

        void submit(std::vector<TransferSubmitInfo> const &submissions, GhaFence *signalFence) override;
    };
}

#include "ValidationQueue.inl"