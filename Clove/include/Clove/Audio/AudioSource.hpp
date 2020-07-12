#pragma once

namespace clv {
    class AudioBuffer;
}

namespace clv {
    class AudioSource {
        //FUNCTIONS
    public:
        virtual ~AudioSource() = default;

        //Sets buffer to play from
        virtual void setBuffer(const AudioBuffer& buffer) = 0;

        //Queues a buffer to be played sequentially
        virtual void queueBuffers(std::vector<std::shared_ptr<AudioBuffer>> buffers)   = 0;
        //Un queues the number of buffers, can get num of unqueuable buffers by calling getNumBuffersProcessed
        virtual std::vector<std::shared_ptr<AudioBuffer>> unQueueBuffers(const uint32_t numToUnqueue) = 0;

        virtual void setPitch(float pitch)      = 0;
        virtual void setLooping(bool isLooping) = 0;

        virtual void setPosition(const clv::mth::vec3f& position) = 0;
        virtual void setVelocity(const clv::mth::vec3f& velocity) = 0;

        virtual clv::mth::vec3f getPosition() const = 0;
        virtual clv::mth::vec3f getVelocity() const = 0;

        virtual uint32_t getNumBuffersQueued() const    = 0;
        virtual uint32_t getNumBuffersProcessed() const = 0;

        virtual void play()   = 0;
        virtual void pause()  = 0;
        virtual void stop()   = 0;
        virtual void rewind() = 0;
    };
}