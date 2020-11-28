#pragma once

#include "Clove/Audio/AudioFactory.hpp"

namespace garlic::clove {
    class ALFactory : public AudioFactory {
        //FUNCTIONS
    public:
        ALFactory();

        ALFactory(ALFactory const &other) = delete;
        ALFactory(ALFactory &&other) noexcept;

        ALFactory &operator=(ALFactory const &other) = delete;
        ALFactory &operator=(ALFactory &&other) noexcept;

        ~ALFactory();

        std::unique_ptr<AudioBuffer> createAudioBuffer(AudioBuffer::Descriptor descriptor) override;
        std::unique_ptr<AudioSource> createAudioSource() override;
        std::unique_ptr<AudioListener> createAudioListener() override;
    };
}
