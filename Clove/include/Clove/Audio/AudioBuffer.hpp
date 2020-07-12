#pragma once

namespace clv {
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

        virtual void map(const DataInfo& info) = 0;
    };
}
