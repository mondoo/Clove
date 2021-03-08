#pragma once

namespace garlic::clove {
    enum class QueueFlags {
        None,
        Transient,   /**< Buffers will be destroyed shortly after using them. */
        ReuseBuffers /**< Buffers will be recorded to mutliple times. */
    };

    struct CommandQueueDescriptor {
        QueueFlags flags;
    };
}