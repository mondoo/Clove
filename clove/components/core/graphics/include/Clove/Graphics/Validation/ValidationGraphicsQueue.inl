#include "Clove/Graphics/Validation/ValidationGraphicsCommandBuffer.hpp"

namespace clove {
    template<typename BaseQueueType>
    std::unique_ptr<GhaGraphicsCommandBuffer> ValidationGraphicsQueue<BaseQueueType>::allocateCommandBuffer() {
        auto commandBuffer{ BaseQueueType::allocateCommandBuffer() };

        bool const allowBufferReuse{ (BaseQueueType::getDescriptor().flags & QueueFlags::ReuseBuffers) != 0 };
        dynamic_cast<ValidationGraphicsCommandbufferInterface *>(commandBuffer.get())->setAllowBufferReuse(allowBufferReuse);

        return commandBuffer;
    }

    template<typename BaseQueueType>
    void ValidationGraphicsQueue<BaseQueueType>::submit(std::vector<GraphicsSubmitInfo> const &submissions, GhaFence *signalFence) {
        BaseQueueType::submit(submissions, signalFence);

        for(auto const &submitInfo : submissions) {
            for(auto &commandBuffer : submitInfo.commandBuffers) {
                dynamic_cast<ValidationGraphicsCommandbufferInterface *>(commandBuffer)->markAsUsed();
            }
        }
    }
}