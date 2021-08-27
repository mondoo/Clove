#include <Clove/Log/Log.hpp>

namespace clove {
    template<typename BaseCommandBufferType>
    void ValidationGraphicsCommandBuffer<BaseCommandBufferType>::beginRecording(CommandBufferUsage usageFlag) {
        CLOVE_ASSERT(!(!allowReuse && hasBeenUsed), "Command buffer re-recorded to. Command buffers cannot only be recorded to more than once unless the owning queue has been created with QueueFlags::ReuseBuffers set.");

        BaseCommandBufferType::beginRecording(usageFlag);
    }

    template<typename BaseCommandBufferType>
    void ValidationGraphicsCommandBuffer<BaseCommandBufferType>::setAllowBufferReuse(bool canReuse) {
        allowReuse = canReuse;
    }

    template<typename BaseCommandBufferType>
    void ValidationGraphicsCommandBuffer<BaseCommandBufferType>::markAsUsed() {
        hasBeenUsed = true;
    }
}