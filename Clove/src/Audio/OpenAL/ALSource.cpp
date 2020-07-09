#include "Clove/Audio/OpenAL/ALSource.hpp"

#include "Clove/Audio/OpenAL/ALBuffer.hpp"
#include "Clove/Audio/OpenAL/ALError.hpp"

namespace clv {
    ALSource::ALSource() {
        alCall(alGenSources(1, &source));
    }

    ALSource::ALSource(ALSource&& other) noexcept = default;

    ALSource& ALSource::operator=(ALSource&& other) noexcept = default;

    ALSource::~ALSource() {
        alCall(alDeleteSources(1, &source));
    }

    void ALSource::setBuffer(const AudioBuffer& buffer) {
        const ALBuffer* alBuffer = safeCast<const ALBuffer>(&buffer);
        alCall(alSourcei(source, AL_BUFFER, alBuffer->getBufferId()));
    }

    void ALSource::setPitch(float pitch) {
        alCall(alSourcef(source, AL_PITCH, pitch));
    }

    void ALSource::setLooping(bool isLooping) {
        alCall(alSourcei(source, AL_LOOPING, isLooping ? AL_TRUE : AL_FALSE));
    }

    void ALSource::setPosition(const mth::vec3f& position) {
        alCall(alSource3f(source, AL_POSITION, position.x, position.y, position.z));
    }

    void ALSource::setVelocity(const mth::vec3f& velocity) {
        alCall(alSource3f(source, AL_VELOCITY, velocity.x, velocity.y, velocity.z));
    }

    mth::vec3f ALSource::getPosition() const {
        mth::vec3f position{};
        alCall(alGetSource3f(source, AL_POSITION, &position.x, &position.y, &position.z));
        return position;
    }

    mth::vec3f ALSource::getVelocity() const {
        mth::vec3f velocity{};
        alCall(alGetSource3f(source, AL_VELOCITY, &velocity.x, &velocity.y, &velocity.z));
        return velocity;
    }

    void ALSource::play() {
        alCall(alSourcePlay(source));
    }

    void ALSource::pause() {
        alCall(alSourcePause(source));
    }

    void ALSource::stop() {
        alCall(alSourceStop(source));
    }

    void ALSource::rewind() {
        alCall(alSourceRewind(source));
    }
}