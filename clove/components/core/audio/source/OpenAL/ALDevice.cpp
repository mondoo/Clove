#include "Clove/Audio/OpenAL/ALDevice.hpp"

#include "Clove/Audio/OpenAL/ALFactory.hpp"

#include <AL/al.h>
#include <Clove/Log/Log.hpp>

CLOVE_DECLARE_LOG_CATEGORY(OPENAL)

namespace garlic::clove {
    namespace {
        void printErrorAlc(ALenum error, std::string_view const fileName, uint_fast32_t const line) {
            switch(error) {
                case ALC_INVALID_VALUE:
                    CLOVE_LOG(LOG_CATEGORY_OPENAL, LogLevel::Error, "ALC_INVALID_VALUE: An invalid value was passed to an OpenAL function. File: {0} Line {1}", fileName, line);
                    break;
                case ALC_INVALID_DEVICE:
                    CLOVE_LOG(LOG_CATEGORY_OPENAL, LogLevel::Error, "ALC_INVALID_DEVICE: A bad device was passed to an OpenAL function. File: {0} Line {1}", fileName, line);
                    break;
                case ALC_INVALID_CONTEXT:
                    CLOVE_LOG(LOG_CATEGORY_OPENAL, LogLevel::Error, "ALC_INVALID_CONTEXT: A bad context was passed to an OpenAL function. File: {0} Line {1}", fileName, line);
                    break;
                case ALC_INVALID_ENUM:
                    CLOVE_LOG(LOG_CATEGORY_OPENAL, LogLevel::Error, "ALC_INVALID_ENUM: An unkown enum was passed to an OpenAL function. File: {0} Line {1}", fileName, line);
                    break;
                case ALC_OUT_OF_MEMORY:
                    CLOVE_LOG(LOG_CATEGORY_OPENAL, LogLevel::Error, "ALC_OUT_OF_MEMORY: The requested operation caused OpenAL to run out of memory. File: {0} Line {1}", fileName, line);
                    break;
            }
        }
    }

#define alcCall(call, device)                         \
    call;                                             \
    {                                                 \
        ALenum error = alcGetError(device);           \
        if(error != AL_NO_ERROR) {                    \
            printErrorAlc(error, __FILE__, __LINE__); \
        }                                             \
    }

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