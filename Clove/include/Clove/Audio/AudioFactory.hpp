#pragma once

#include "Clove/Audio/AudioBuffer.hpp"
#include "Clove/Audio/AudioListener.hpp"
#include "Clove/Audio/AudioSource.hpp"

namespace clv {
    class AudioFactory {
        //FUNCTIONS
    public:
        virtual ~AudioFactory() = default;

        virtual std::unique_ptr<AudioBuffer> createAudioBuffer() = 0;
        virtual std::unique_ptr<AudioBuffer> createAudioBuffer(const AudioDataDescriptor& descriptor) = 0;
        virtual std::unique_ptr<AudioSource> createAudioSource() = 0;
        virtual std::unique_ptr<AudioListener> createAudioListener() = 0;
    };
}