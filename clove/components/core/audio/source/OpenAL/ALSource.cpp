#include "Clove/Audio/OpenAL/ALSource.hpp"

#include "Clove/Audio/OpenAL/ALBuffer.hpp"
#include "Clove/Audio/OpenAL/ALError.hpp"

#include <Clove/Cast.hpp>
#include <Clove/Definitions.hpp>
#include <Clove/Log/Log.hpp>
#include <set>

namespace garlic::clove {
    ALSource::ALSource(ALuint source)
        : source{ source  } {
    }

    ALSource::ALSource(ALSource &&other) noexcept = default;

    ALSource &ALSource::operator=(ALSource &&other) noexcept = default;

    ALSource::~ALSource() {
        alCall(alDeleteSources(1, &source));
    }

    void ALSource::setBuffer(std::shared_ptr<AudioBuffer> buffer) {
        ALBuffer const *alBuffer{ polyCast<ALBuffer const>(buffer.get()) };
        alCall(alSourcei(source, AL_BUFFER, alBuffer->getBufferId()));

        bufferQueue = { buffer };
    }

    void ALSource::queueBuffers(std::vector<std::shared_ptr<AudioBuffer>> buffers) {
        //Get the buffer Id of all of the buffers to placed into the queue
        std::vector<ALuint> alBuffers(std::size(buffers));
        for(size_t i = 0; i < std::size(alBuffers); ++i) {
            ALBuffer const *alBuffer{ polyCast<ALBuffer const>(buffers[i].get()) };
            alBuffers[i] = alBuffer->getBufferId();
        }

        alCall(alSourceQueueBuffers(source, std::size(alBuffers), alBuffers.data()));

        bufferQueue.insert(std::end(bufferQueue), std::begin(buffers), std::end(buffers));
    }

    std::vector<std::shared_ptr<AudioBuffer>> ALSource::unQueueBuffers(uint32_t const numToUnqueue) {
#if CLOVE_DEBUG
        const uint32_t maxAbleToUnQueue = getNumBuffersProcessed();
        CLOVE_ASSERT(numToUnqueue <= maxAbleToUnQueue, "{0}, Can't unqueue {1} buffers. Only {2} buffers have been processed", CLOVE_FUNCTION_NAME_PRETTY, numToUnqueue, maxAbleToUnQueue);
#endif

        ALuint *buffers = new ALuint[numToUnqueue];
        alCall(alSourceUnqueueBuffers(source, numToUnqueue, buffers));

        std::set<ALuint> pendingBuffers;
        for(size_t i = 0; i < numToUnqueue; i++) {
            pendingBuffers.emplace(buffers[i]);
        }

        delete[] buffers;

        std::vector<std::shared_ptr<AudioBuffer>> removedBuffers;
        removedBuffers.reserve(numToUnqueue);

        auto removeIter = std::remove_if(std::begin(bufferQueue), std::end(bufferQueue), [&](std::shared_ptr<AudioBuffer> const &buffer) {
            ALBuffer const *alBuffer{ polyCast<const ALBuffer>(buffer.get()) };
            ALuint const bufferId{ alBuffer->getBufferId() };

            if(pendingBuffers.find(bufferId) != pendingBuffers.end()) {
                removedBuffers.push_back(buffer);
                return true;
            }

            return false;
        });

        bufferQueue.erase(removeIter, std::end(bufferQueue));

        return removedBuffers;
    }

    void ALSource::setPitch(float pitch) {
        alCall(alSourcef(source, AL_PITCH, pitch));
    }

    void ALSource::setLooping(bool isLooping) {
        alCall(alSourcei(source, AL_LOOPING, isLooping ? AL_TRUE : AL_FALSE));
    }

    void ALSource::setPosition(vec3f const &position) {
        alCall(alSource3f(source, AL_POSITION, position.x, position.y, position.z));
    }

    void ALSource::setVelocity(vec3f const &velocity) {
        alCall(alSource3f(source, AL_VELOCITY, velocity.x, velocity.y, velocity.z));
    }

    vec3f ALSource::getPosition() const {
        vec3f position{};
        alCall(alGetSource3f(source, AL_POSITION, &position.x, &position.y, &position.z));
        return position;
    }

    vec3f ALSource::getVelocity() const {
        vec3f velocity{};
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