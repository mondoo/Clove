#include "Clove/Audio/OpenAL/ALBuffer.hpp"

#include "Clove/Audio/OpenAL/ALError.hpp"

namespace garlic::clove {
    static ALenum convertFormat(AudioBuffer::Format format) {
        switch(format) {
            case AudioBuffer::Format::Mono8:
                return AL_FORMAT_MONO8;
            case AudioBuffer::Format::Mono16:
                return AL_FORMAT_MONO16;
            case AudioBuffer::Format::Stereo8:
                return AL_FORMAT_STEREO8;
            case AudioBuffer::Format::Stereo16:
                return AL_FORMAT_STEREO16;
        }
    }

    ALBuffer::ALBuffer(Descriptor descriptor)
        : descriptor(std::move(descriptor)) {
        alCall(alGenBuffers(1, &buffer));
    }

    ALBuffer::ALBuffer(ALBuffer &&other) noexcept = default;

    ALBuffer &ALBuffer::operator=(ALBuffer &&other) noexcept = default;

    ALBuffer::~ALBuffer() {
        alCall(alDeleteBuffers(1, &buffer));
    }

    void ALBuffer::write(void const *data, size_t const size) {
        alCall(alBufferData(buffer, convertFormat(descriptor.format), data, size, descriptor.sampleRate));
    }

    ALuint ALBuffer::getBufferId() const {
        return buffer;
    }
}
