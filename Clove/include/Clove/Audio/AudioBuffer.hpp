#pragma once

namespace clv {
    enum class AudioFormat {
        Mono8,
        Mono16,
        Stereo8,
        Stereo16
    };

    struct AudioDataDescriptor {
        AudioFormat format;
        size_t sampleRate = 0;
        const void* data  = nullptr;
        size_t dataSize   = 0;
    };
}

namespace clv {
    class AudioBuffer {
        //FUNCTIONS
    public:
        virtual ~AudioBuffer() = default;

        virtual void map(const AudioDataDescriptor& descriptor) = 0;
    };
}