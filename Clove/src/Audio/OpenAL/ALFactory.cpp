#include "Clove/Audio/OpenAL/ALFactory.hpp"

#include "Clove/Audio/OpenAL/ALBuffer.hpp"

namespace clv {
    ALFactory::ALFactory(){
        alDevice = alcOpenDevice(nullptr);
        if(!alDevice) {
            GARLIC_LOG(garlicLogContext, Log::Level::Error, "Failed to create OpenAL device");
            return;
        }

        alContext = alcCreateContext(alDevice, nullptr);
        alcMakeContextCurrent(alContext);
    }

    ALFactory::ALFactory(ALFactory&& other) noexcept = default;

    ALFactory& ALFactory::operator=(ALFactory&& other) noexcept = default;

    ALFactory::~ALFactory(){
        alcMakeContextCurrent(nullptr);
        alcDestroyContext(alContext);

        alcCloseDevice(alDevice);
    }

    std::unique_ptr<AudioBuffer> ALFactory::createAudioBuffer(AudioBufferDescriptor descriptor, const void* data, size_t dataSize) {
        return std::make_unique<ALBuffer>(descriptor, data, dataSize);
    }
}