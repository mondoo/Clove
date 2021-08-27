#include "Clove/Graphics/Validation/ValidationCommandBuffer.hpp"

#include <Clove/Log/Log.hpp>

namespace clove {
    void ValidationCommandBuffer::setAllowBufferReuse(bool canReuse) {
        allowReuse = canReuse;
    }

    void ValidationCommandBuffer::markAsUsed() {
        hasBeenUsed = true;
    }

    void ValidationCommandBuffer::validateBeginRecording() {
        CLOVE_ASSERT(!(!allowReuse && hasBeenUsed), "Command buffer re-recorded to. Command buffers cannot only be recorded to more than once unless the owning queue has been created with QueueFlags::ReuseBuffers set.");
    }
}