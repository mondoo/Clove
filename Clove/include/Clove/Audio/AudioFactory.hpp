#pragma once

#include "Clove/Audio/AudioBuffer.hpp"

namespace clv {
    class AudioFactory {
        //FUNCTIONS
    public:
        virtual ~AudioFactory() = default;

        virtual std::unique_ptr<AudioBuffer> createAudioBuffer(AudioBufferDescriptor descriptor, const void* data, size_t dataSize) = 0;
    };
}