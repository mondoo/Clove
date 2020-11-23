#pragma once

namespace clv::gfx {
    enum class QueueFlags {
        None,
        Transient,   /**< Buffers will be desrtoyed short after using them. */
        ReuseBuffers /**< Buffers will be recorded to mutliple times. */
    };

    struct CommandQueueDescriptor {
        QueueFlags flags;
    };
}