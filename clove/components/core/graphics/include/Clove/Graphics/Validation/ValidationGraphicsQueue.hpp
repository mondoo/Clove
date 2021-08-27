#pragma once

#include "Clove/Graphics/GhaGraphicsQueue.hpp"

namespace clove {
    template<typename BaseQueueType>
    class ValidationGraphicsQueue : public BaseQueueType {
        //FUNCTIONS
    public:
        using BaseQueueType::BaseQueueType;

        std::unique_ptr<GhaGraphicsCommandBuffer> allocateCommandBuffer() override;

        void submit(std::vector<GraphicsSubmitInfo> const &submissions, GhaFence *signalFence) override;
    };
}

#include "ValidationGraphicsQueue.inl"