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

    void ALSource::queueBuffer(const AudioBuffer& buffer) {
        const ALBuffer* alBuffer = safeCast<const ALBuffer>(&buffer);
        const ALuint buffers[]   = { alBuffer->getBufferId() };
        alCall(alSourceQueueBuffers(source, 1, buffers));
    }

    void ALSource::unQueueBuffer(const AudioBuffer& buffer) {
        const ALBuffer* alBuffer = safeCast<const ALBuffer>(&buffer);
        ALuint buffers[]         = { alBuffer->getBufferId() };
        alCall(alSourceUnqueueBuffers(source, 1, buffers));
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

    uint32_t ALSource::getNumBuffersQueued() const {
        ALint buffersQeued = 0;
        alCall(alGetSourcei(source, AL_BUFFERS_QUEUED, &buffersQeued));
        return static_cast<uint32_t>(buffersQeued);
    }

    uint32_t ALSource::getNumBuffersProcessed() const {
        ALint buffersProcessed = 0;
        alCall(alGetSourcei(source, AL_BUFFERS_PROCESSED, &buffersProcessed));
        return static_cast<uint32_t>(buffersProcessed);
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