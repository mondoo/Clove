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

        Expected<std::unique_ptr<AudioBuffer>, std::exception> createAudioBuffer(AudioBuffer::Descriptor descriptor) override;
        Expected<std::unique_ptr<AudioSource>, std::exception> createAudioSource() override;
        Expected<std::unique_ptr<AudioListener>, std::exception> createAudioListener() override;
    };
}
