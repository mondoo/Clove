#include "Clove/Audio/OpenAL/ALBuffer.hpp"

#include "Clove/Audio/OpenAL/ALError.hpp"

namespace clv {
    static ALenum convertFormat(BufferFormat format) {
        switch(format) {
            case BufferFormat::Mono8:
                return AL_FORMAT_MONO8;
            case BufferFormat::Mono16:
                return AL_FORMAT_MONO16;
            case BufferFormat::Stereo8:
                return AL_FORMAT_STEREO8;
            case BufferFormat::Stereo16:
                return AL_FORMAT_STEREO16;
        }
    }

    ALBuffer::ALBuffer(AudioBufferDescriptor descriptor, const void* data, size_t dataSize) {
        alCall(alGenBuffers(1, &buffer));
        alCall(alBufferData(buffer, convertFormat(descriptor.format), data, dataSize, descriptor.sampleRate));
    }

    ALBuffer::ALBuffer(ALBuffer&& other) noexcept = default;

    ALBuffer& ALBuffer::operator=(ALBuffer&& other) noexcept = default;

    ALBuffer::~ALBuffer() {
        alCall(alDeleteBuffers(1, &buffer));
    }

    ALuint ALBuffer::getBufferId() const {
        return buffer;
    }
}