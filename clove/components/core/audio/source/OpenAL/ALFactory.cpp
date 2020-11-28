#include "Clove/Audio/OpenAL/ALFactory.hpp"

#include "Clove/Audio/OpenAL/ALBuffer.hpp"
#include "Clove/Audio/OpenAL/ALListener.hpp"
#include "Clove/Audio/OpenAL/ALSource.hpp"

namespace garlic::clove {
    ALFactory::ALFactory() = default;

    ALFactory::ALFactory(ALFactory &&other) noexcept = default;

    ALFactory &ALFactory::operator=(ALFactory &&other) noexcept = default;

    ALFactory::~ALFactory() = default;

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
