#pragma once

#include "Clove/Audio/AudioFactory.hpp"

namespace clv {
    class ALFactory : public AudioFactory {
        //FUNCTIONS
    public:
        //TODO: Ctors

        std::unique_ptr<AudioBuffer> createAudioBuffer(AudioBufferDescriptor descriptor, const void* data, size_t dataSize) override;
    };
}