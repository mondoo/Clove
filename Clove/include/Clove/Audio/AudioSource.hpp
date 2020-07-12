#pragma once

namespace clv {
    class AudioBuffer;
}

namespace clv {
	/**
	 * @brief An AudioSource allows sound to played from a cetain point in space
	 */
    class AudioSource {
        //FUNCTIONS
    public:
        virtual ~AudioSource() = default;

        /**
		 * @brief Sets buffer to play audio from
		 */
        virtual void setBuffer(const AudioBuffer& buffer) = 0;

        /**
		 * @brief Queues buffers to be played sequentially.
		 * @details Buffers are appended to the end of the current queue.
		 * @param buffers Vector of buffers to add to the queue.
		 */
        virtual void queueBuffers(std::vector<std::shared_ptr<AudioBuffer>> buffers)   = 0;
        /**
		 * @brief Removes buffers from the queue.
		 * @details numToUnQueue must be <= getNumBuffersProcessed.
		 * @param numToQueue The number of buffers to be removed from the queue.
		 * @returns A vector of AudioBuffers removed from the queue.
		 */
		virtual std::vector<std::shared_ptr<AudioBuffer>> unQueueBuffers(const uint32_t numToUnqueue) = 0;

        virtual void setPitch(float pitch)      = 0;
        virtual void setLooping(bool isLooping) = 0;

        virtual void setPosition(const clv::mth::vec3f& position) = 0;
        virtual void setVelocity(const clv::mth::vec3f& velocity) = 0;

        virtual clv::mth::vec3f getPosition() const = 0;
        virtual clv::mth::vec3f getVelocity() const = 0;

		/**
		 * @brief How many buffers are in the queue (not played or playing)
		 */
        virtual uint32_t getNumBuffersQueued() const    = 0;
		/**
		 * @brief How many buffers have been played
		 */
        virtual uint32_t getNumBuffersProcessed() const = 0;

        virtual void play()   = 0;
        virtual void pause()  = 0;
        virtual void stop()   = 0;
        virtual void rewind() = 0;
    };
}
