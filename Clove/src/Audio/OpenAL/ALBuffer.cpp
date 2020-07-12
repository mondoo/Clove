#include "Clove/Audio/OpenAL/ALBuffer.hpp"

#include "Clove/Audio/OpenAL/ALError.hpp"

namespace clv {
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

    ALBuffer::ALBuffer() {
        alCall(alGenBuffers(1, &buffer));
    }

    ALBuffer::ALBuffer(const DataInfo& info)
        : ALBuffer(){
        map(info);
    }

    ALBuffer::ALBuffer(ALBuffer&& other) noexcept = default;

    ALBuffer& ALBuffer::operator=(ALBuffer&& other) noexcept = default;

    ALBuffer::~ALBuffer() {
        alCall(alDeleteBuffers(1, &buffer));
    }

    void ALBuffer::map(const DataInfo& info) {
        alCall(alBufferData(buffer, convertFormat(info.format), info.data, info.dataSize, info.sampleRate));
    }

    ALuint ALBuffer::getBufferId() const {
        return buffer;
    }
}
