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
		
		struct DataInfo {
			Format format;
			size_t sampleRate = 0;
			const void* data  = nullptr;
			size_t dataSize   = 0;
		};
		
        //FUNCTIONS
    public:
        virtual ~AudioBuffer() = default;

		/**
		 * @brief Write data into the buffer.
		 */
        virtual void map(const DataInfo& info) = 0;
    };
}
