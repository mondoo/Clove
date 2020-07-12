#pragma once

#include "Clove/Audio/AudioFactory.hpp"

#include <AL/alc.h>

namespace clv {
    class ALFactory : public AudioFactory {
        //VARIABLES
    private:
        ALCdevice* alDevice = nullptr;
        ALCcontext* alContext = nullptr;

        //FUNCTIONS
    public:
        ALFactory();

        ALFactory(const ALFactory& other) = delete;
        ALFactory(ALFactory&& other) noexcept;

        ALFactory& operator=(const ALFactory& other) = delete;
        ALFactory& operator=(ALFactory&& other) noexcept;

        ~ALFactory();

        std::unique_ptr<AudioBuffer> createAudioBuffer() override;
        std::unique_ptr<AudioBuffer> createAudioBuffer(const AudioBuffer::DataInfo& info) override;
        std::unique_ptr<AudioSource> createAudioSource() override;
        std::unique_ptr<AudioListener> createAudioListener() override;
    };
}
