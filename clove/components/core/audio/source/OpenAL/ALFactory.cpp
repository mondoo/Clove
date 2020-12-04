#include "Clove/Audio/OpenAL/ALFactory.hpp"

#include "Clove/Audio/OpenAL/ALBuffer.hpp"
#include "Clove/Audio/OpenAL/ALError.hpp"
#include "Clove/Audio/OpenAL/ALListener.hpp"
#include "Clove/Audio/OpenAL/ALSource.hpp"

namespace garlic::clove {
    ALFactory::ALFactory() = default;

    ALFactory::ALFactory(ALFactory &&other) noexcept = default;

    ALFactory &ALFactory::operator=(ALFactory &&other) noexcept = default;

    ALFactory::~ALFactory() = default;

    Expected<std::unique_ptr<AudioBuffer>, std::runtime_error> ALFactory::createAudioBuffer(AudioBuffer::Descriptor descriptor) {
        ALuint buffer;
        alGenBuffers(1, &buffer);

        ALenum const error{ alGetError() };
        if(error != AL_NO_ERROR) {
            printErrorAl(error, __FILE__, __LINE__);
            return Unexpected{ std::runtime_error{ "Failed to create AudioBuffer. See log for details." } };
        }

        return std::unique_ptr<AudioBuffer>{ std::make_unique<ALBuffer>(buffer, std::move(descriptor)) };
    }

    Expected<std::unique_ptr<AudioSource>, std::runtime_error> ALFactory::createAudioSource() {
        ALuint source;
        alGenSources(1, &source);

        ALenum const error{ alGetError() };
        if(error != AL_NO_ERROR){
            printErrorAl(error, __FILE__, __LINE__);
            return Unexpected{ std::runtime_error{ "Failed to create AudioSource. See log for details." } };
        }

        return std::unique_ptr<AudioSource>{ std::make_unique<ALSource>(source) };
    }

    Expected<std::unique_ptr<AudioListener>, std::runtime_error> ALFactory::createAudioListener() {
        return std::unique_ptr<AudioListener>{ std::make_unique<ALListener>() };
    }
}
