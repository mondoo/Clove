#pragma once

#include <sndfile.h> //TODO: move to cpp

namespace clv::aud{
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