#pragma once

namespace clv {
	/**
	 * @brief Represents a buffer of data that can be played with an AudioSource
	 * @details Created from an AudioFactory. Can be created empty and then
	 * updated later.
	 */
    class AudioBuffer {
		//TYPES
	public:
		enum class Format {
			Mono8,
			Mono16,
			Stereo8,
			Stereo16
		};

		struct Descriptor{
            Format format;
            size_t sampleRate{ 0 };
        };

        //FUNCTIONS
    public:
        virtual ~AudioBuffer() = default;

        /**
         * @brief Write data into the buffer.
         * @param data A pointer to the data that will be written to this buffer.
         * @param size Size of the region to write to.
         */
        virtual void write(void const *data, size_t const size) = 0;
    };
}
