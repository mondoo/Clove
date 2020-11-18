#pragma once

#include "Clove/Audio/AudioFactory.hpp"

#include <AL/alc.h>

namespace clv {
    class ALFactory : public AudioFactory {
        //VARIABLES
    private:
        ALCdevice *alDevice{ nullptr };
        ALCcontext *alContext{ nullptr };

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
