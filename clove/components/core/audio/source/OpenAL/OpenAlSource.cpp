#include "Clove/Audio/OpenAL/OpenAlSource.hpp"

#include "Clove/Audio/OpenAL/OpenAlBuffer.hpp"
#include "Clove/Audio/OpenAL/OpenAlError.hpp"

#include <Clove/Cast.hpp>
#include <Clove/Definitions.hpp>
#include <Clove/Log/Log.hpp>
#include <set>

namespace garlic::clove {
    OpenAlSource::OpenAlSource(ALuint source)
        : source{ source  } {
    }

    OpenAlSource::OpenAlSource(OpenAlSource &&other) noexcept = default;

    OpenAlSource &OpenAlSource::operator=(OpenAlSource &&other) noexcept = default;

    OpenAlSource::~OpenAlSource() {
        alCall(alDeleteSources(1, &source));
    }

    void OpenAlSource::setBuffer(std::shared_ptr<AhaBuffer> buffer) {
        OpenAlBuffer const *alBuffer{ polyCast<OpenAlBuffer const>(buffer.get()) };
        alCall(alSourcei(source, AL_BUFFER, alBuffer->getBufferId()));

        bufferQueue = { buffer };
    }

    void OpenAlSource::queueBuffers(std::vector<std::shared_ptr<AhaBuffer>> buffers) {
        //Get the buffer Id of all of the buffers to placed into the queue
        std::vector<ALuint> alBuffers(std::size(buffers));
        for(size_t i = 0; i < std::size(alBuffers); ++i) {
            OpenAlBuffer const *alBuffer{ polyCast<OpenAlBuffer const>(buffers[i].get()) };
            alBuffers[i] = alBuffer->getBufferId();
        }

        alCall(alSourceQueueBuffers(source, std::size(alBuffers), alBuffers.data()));

        bufferQueue.insert(std::end(bufferQueue), std::begin(buffers), std::end(buffers));
    }

    std::vector<std::shared_ptr<AhaBuffer>> OpenAlSource::unQueueBuffers(uint32_t const numToUnqueue) {
#if CLOVE_DEBUG
        const uint32_t maxAbleToUnQueue = getNumBuffersProcessed();
        CLOVE_ASSERT(numToUnqueue <= maxAbleToUnQueue, "{0}, Can't unqueue {1} buffers. Only {2} buffers have been processed", CLOVE_FUNCTION_NAME_PRETTY, numToUnqueue, maxAbleToUnQueue);
#endif

        auto *buffers = new ALuint[numToUnqueue];
        alCall(alSourceUnqueueBuffers(source, numToUnqueue, buffers));

        std::set<ALuint> pendingBuffers;
        for(size_t i = 0; i < numToUnqueue; i++) {
            pendingBuffers.emplace(buffers[i]);
        }

        delete[] buffers;

        std::vector<std::shared_ptr<AhaBuffer>> removedBuffers;
        removedBuffers.reserve(numToUnqueue);

        auto removeIter = std::remove_if(std::begin(bufferQueue), std::end(bufferQueue), [&](std::shared_ptr<AhaBuffer> const &buffer) {
            OpenAlBuffer const *alBuffer{ polyCast<const OpenAlBuffer>(buffer.get()) };
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

    void OpenAlSource::setPitch(float pitch) {
        alCall(alSourcef(source, AL_PITCH, pitch));
    }

    void OpenAlSource::setLooping(bool isLooping) {
        alCall(alSourcei(source, AL_LOOPING, isLooping ? AL_TRUE : AL_FALSE));
    }

    void OpenAlSource::setPosition(vec3f const &position) {
        alCall(alSource3f(source, AL_POSITION, position.x, position.y, position.z));
    }

    void OpenAlSource::setVelocity(vec3f const &velocity) {
        alCall(alSource3f(source, AL_VELOCITY, velocity.x, velocity.y, velocity.z));
    }

    vec3f OpenAlSource::getPosition() const {
        vec3f position{};
        alCall(alGetSource3f(source, AL_POSITION, &position.x, &position.y, &position.z));
        return position;
    }

    vec3f OpenAlSource::getVelocity() const {
        vec3f velocity{};
        alCall(alGetSource3f(source, AL_VELOCITY, &velocity.x, &velocity.y, &velocity.z));
        return velocity;
    }

    uint32_t OpenAlSource::getNumBuffersQueued() const {
        ALint buffersQeued = 0;
        alCall(alGetSourcei(source, AL_BUFFERS_QUEUED, &buffersQeued));
        return static_cast<uint32_t>(buffersQeued);
    }

    uint32_t OpenAlSource::getNumBuffersProcessed() const {
        ALint buffersProcessed = 0;
        alCall(alGetSourcei(source, AL_BUFFERS_PROCESSED, &buffersProcessed));
        return static_cast<uint32_t>(buffersProcessed);
    }

    void OpenAlSource::play() {
        alCall(alSourcePlay(source));
    }

    void OpenAlSource::pause() {
        alCall(alSourcePause(source));
    }

    void OpenAlSource::stop() {
        alCall(alSourceStop(source));
    }

    void OpenAlSource::rewind() {
        alCall(alSourceRewind(source));
    }
}