#pragma once

namespace clv::gfx {
    enum class QueueFlags {
        None,
        Transient,   /**< Buffers will be short lived */
        ReuseBuffers /**< Buffers can be reused */
    };

    struct CommandQueueDescriptor {
        QueueFlags flags;
    };
}