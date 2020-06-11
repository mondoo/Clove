#pragma once

namespace clv {
    enum class BufferFormat {
        Mono8,
        Mono16,
        Stereo8,
        Stereo16
    };

    struct AudioBufferDescriptor {
        BufferFormat format;
        size_t sampleRate;
    };
}

namespace clv {
    class AudioBuffer {
        //FUNCTIONS
    public:
        virtual ~AudioBuffer() = default;
    };
}