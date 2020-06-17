#include "Clove/Audio/OpenAL/ALSource.hpp"

#include "Clove/Audio/OpenAL/ALBuffer.hpp"

namespace clv {
    ALSource::ALSource() {
        alGenSources(1, &source);
    }

    ALSource::ALSource(ALSource&& other) noexcept = default;

    ALSource& ALSource::operator=(ALSource&& other) noexcept = default;

    ALSource::~ALSource() {
        alDeleteSources(1, &source);
    }

    void ALSource::setBuffer(const AudioBuffer& buffer) {
        const ALBuffer* alBuffer = safeCast<const ALBuffer>(&buffer);
        alSourcei(source, AL_BUFFER, alBuffer->getBufferId());
    }

    void ALSource::setPitch(float pitch) {
        alSourcef(source, AL_PITCH, pitch);
    }

    void ALSource::setLooping(bool isLooping) {
        alSourcei(source, AL_LOOPING, isLooping ? AL_TRUE : AL_FALSE);
    }

    void ALSource::setPosition(const clv::mth::vec3f& position) {
        alSource3f(source, AL_POSITION, position.x, position.y, position.z);
    }

    void ALSource::setVelocity(const clv::mth::vec3f& velocity) {
        alSource3f(source, AL_VELOCITY, velocity.x, velocity.y, velocity.z);
    }

    void ALSource::play() {
        alSourcePlay(source);
    }

    void ALSource::pause() {
        alSourcePause(source);
    }

    void ALSource::stop() {
        alSourceStop(source);
    }

    void ALSource::rewind() {
        alSourceRewind(source);
    }
}