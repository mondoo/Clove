#include "Clove/Graphics/Validation/ValidationCommandBuffer.hpp"

#include <Clove/Log/Log.hpp>

namespace clove {
    void ValidationCommandBuffer::setAllowBufferReuse(bool canReuse) {
        allowReuse = canReuse;
    }

    void ValidationCommandBuffer::markAsUsed() {
        hasBeenUsed = true;
    }

    bool ValidationCommandBuffer::bufferHasBeenUsed() {
        return hasBeenUsed;
    }

    CommandBufferUsage ValidationCommandBuffer::getCommandBufferUsage() const {
        return currentUsage;
    }

    void ValidationCommandBuffer::validateBeginRecording() {
        CLOVE_ASSERT(!(!allowReuse && hasBeenUsed), "Command buffer re-recorded to. Command buffers cannot be recorded to more than once unless the owning queue has been created with QueueFlags::ReuseBuffers set.");
    }

    void ValidationCommandBuffer::resetUsedFlag() {
        hasBeenUsed = false;
    }

    void ValidationCommandBuffer::setCommandBufferUsage(CommandBufferUsage usage) {
        currentUsage = usage;
    }

    void ValidationCommandBuffer::validateBeginRecording() {
        if(!endRecordingCalled) {
            CLOVE_ASSERT("beginRecording called before endRecording. Command buffer recording must be finished be starting again.");
        }
        endRecordingCalled = false;
    }

    void ValidationCommandBuffer::endRecording() {
        endRecordingCalled = true;
    }
}

