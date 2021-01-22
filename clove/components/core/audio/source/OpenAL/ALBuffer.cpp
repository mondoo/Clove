#include "Clove/Audio/OpenAL/ALBuffer.hpp"

#include "Clove/Audio/OpenAL/ALError.hpp"

#include <utility>

namespace garlic::clove {
    namespace {
        ALenum convertFormat(AhaBuffer::Format format) {
            switch(format) {
                case AhaBuffer::Format::Mono8:
                    return AL_FORMAT_MONO8;
                case AhaBuffer::Format::Mono16:
                    return AL_FORMAT_MONO16;
                case AhaBuffer::Format::Stereo8:
                    return AL_FORMAT_STEREO8;
                case AhaBuffer::Format::Stereo16:
                    return AL_FORMAT_STEREO16;
            }
        }
    }

    ALBuffer::ALBuffer(ALuint buffer, Descriptor descriptor)
        : buffer{ buffer }
        , descriptor{ std::move(descriptor) } {
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
