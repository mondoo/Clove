#include "Clove/Audio/OpenAL/ALDevice.hpp"

#include "Clove/Audio/OpenAL/ALError.hpp"
#include "Clove/Audio/OpenAL/ALFactory.hpp"

#include <Clove/Log/Log.hpp>

namespace garlic::clove {
    ALDevice::ALDevice()
        : factory{ std::make_shared<ALFactory>() } {
        alDevice = alcOpenDevice(nullptr);
        if(!alDevice) {
            CLOVE_LOG(LOG_CATEGORY_CLOVE, LogLevel::Error, "Failed to create OpenAL device");
            return;
        }

        alcCall(alContext = alcCreateContext(alDevice, nullptr), alDevice);
        alcCall(alcMakeContextCurrent(alContext), alDevice);
    }

    ALDevice::ALDevice(ALDevice &&other) noexcept = default;

    ALDevice &ALDevice::operator=(ALDevice &&other) noexcept = default;

    ALDevice::~ALDevice() {
        alcCall(alcMakeContextCurrent(nullptr), alDevice);
        alcCall(alcDestroyContext(alContext), alDevice);

        alcCloseDevice(alDevice);
    }

    std::shared_ptr<AudioFactory> ALDevice::getAudioFactory() const {
        return factory;
    }
}