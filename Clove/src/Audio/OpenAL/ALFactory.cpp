#include "Clove/Audio/OpenAL/ALFactory.hpp"

#include "Clove/Audio/OpenAL/ALBuffer.hpp"

namespace clv {
    std::unique_ptr<AudioBuffer> ALFactory::createAudioBuffer(AudioBufferDescriptor descriptor, const void* data, size_t dataSize) {
        return std::make_unique<ALBuffer>(descriptor, data, dataSize);
    }
}