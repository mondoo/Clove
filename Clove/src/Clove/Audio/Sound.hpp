#pragma once

#include <sndfile.h> //TODO: cpp?

namespace clv::sfx{
	class Sound{
		//VARIABLES
	private:
		SNDFILE* file;
		SF_INFO info;

		//FUNCTIONS
	public:
		Sound() = delete;
		Sound(const Sound& other);
		Sound& operator=(const Sound& other);
		Sound(Sound&& other) noexcept;
		Sound& operator=(Sound&& other) noexcept;
		~Sound();

		Sound(const std::string &filePath);

		int32 getChannels() const;
		int32 getSampleRate() const;

		SNDFILE* getFile() const;
	};
}