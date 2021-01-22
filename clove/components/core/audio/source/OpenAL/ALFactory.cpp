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

    Expected<std::unique_ptr<AhaBuffer>, std::runtime_error> ALFactory::createAudioBuffer(AhaBuffer::Descriptor descriptor) {
        ALuint buffer;
        alGenBuffers(1, &buffer);

        ALenum const error{ alGetError() };
        if(error != AL_NO_ERROR) {
            printErrorAl(error, __FILE__, __LINE__);
            return Unexpected{ std::runtime_error{ "Failed to create AhaBuffer. See log for details." } };
        }

        return std::unique_ptr<AhaBuffer>{ std::make_unique<ALBuffer>(buffer, std::move(descriptor)) };
    }

    Expected<std::unique_ptr<AhaSource>, std::runtime_error> ALFactory::createAudioSource() {
        ALuint source;
        alGenSources(1, &source);

        ALenum const error{ alGetError() };
        if(error != AL_NO_ERROR){
            printErrorAl(error, __FILE__, __LINE__);
            return Unexpected{ std::runtime_error{ "Failed to create AhaSource. See log for details." } };
        }

        return std::unique_ptr<AhaSource>{ std::make_unique<ALSource>(source) };
    }

    Expected<std::unique_ptr<AhaListener>, std::runtime_error> ALFactory::createAudioListener() {
        return std::unique_ptr<AhaListener>{ std::make_unique<ALListener>() };
    }
}
