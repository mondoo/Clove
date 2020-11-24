#include "Clove/Audio/OpenAL/ALFactory.hpp"

#include "Clove/Audio/OpenAL/ALBuffer.hpp"
#include "Clove/Audio/OpenAL/ALError.hpp"
#include "Clove/Audio/OpenAL/ALListener.hpp"
#include "Clove/Audio/OpenAL/ALSource.hpp"

#include <Clove/Log/Log.hpp>

namespace garlic::clove {
    ALFactory::ALFactory() {
        alDevice = alcOpenDevice(nullptr);
        if(!alDevice) {
            GARLIC_LOG(garlicLogContext, LogLevel::Error, "Failed to create OpenAL device");
            return;
        }

        alcCall(alContext = alcCreateContext(alDevice, nullptr), alDevice);
        alcCall(alcMakeContextCurrent(alContext), alDevice);
    }

    ALFactory::ALFactory(ALFactory &&other) noexcept = default;

    ALFactory &ALFactory::operator=(ALFactory &&other) noexcept = default;

    ALFactory::~ALFactory() {
        alcCall(alcMakeContextCurrent(nullptr), alDevice);
        alcCall(alcDestroyContext(alContext), alDevice);

        alcCloseDevice(alDevice);
    }

    std::unique_ptr<AudioBuffer> ALFactory::createAudioBuffer(AudioBuffer::Descriptor descriptor) {
        return std::make_unique<ALBuffer>(std::move(descriptor));
    }

    std::unique_ptr<AudioSource> ALFactory::createAudioSource() {
        return std::make_unique<ALSource>();
    }

    std::unique_ptr<AudioListener> ALFactory::createAudioListener() {
        return std::make_unique<ALListener>();
    }
}
