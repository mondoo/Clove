#include "Clove/Audio/OpenAL/ALBuffer.hpp"

#include "Clove/Audio/OpenAL/ALError.hpp"

namespace clv {
    static ALenum convertFormat(AudioFormat format) {
        switch(format) {
            case AudioFormat::Mono8:
                return AL_FORMAT_MONO8;
            case AudioFormat::Mono16:
                return AL_FORMAT_MONO16;
            case AudioFormat::Stereo8:
                return AL_FORMAT_STEREO8;
            case AudioFormat::Stereo16:
                return AL_FORMAT_STEREO16;
        }
    }

    ALBuffer::ALBuffer() {
        alCall(alGenBuffers(1, &buffer));
    }

    ALBuffer::ALBuffer(const AudioDataDescriptor& descriptor) {
        alCall(alGenBuffers(1, &buffer));
        map(descriptor);
    }

    ALBuffer::ALBuffer(ALBuffer&& other) noexcept = default;

    ALBuffer& ALBuffer::operator=(ALBuffer&& other) noexcept = default;

    ALBuffer::~ALBuffer() {
        alCall(alDeleteBuffers(1, &buffer));
    }

    void ALBuffer::map(const AudioDataDescriptor& descriptor) {
        alCall(alBufferData(buffer, convertFormat(descriptor.format), descriptor.data, descriptor.dataSize, descriptor.sampleRate));
    }

    ALuint ALBuffer::getBufferId() const {
        return buffer;
    }
}