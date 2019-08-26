#pragma once

#include <sndfile.h> //TODO: move to cpp

namespace clv::aud{
	class Sound{
		//VARIABLES
	private:
		SNDFILE* file;
		SF_INFO info;
		//Again, temp
	public:
	int32 position;

		//FUNCTIONS
	public:
		Sound() = default; //Temp default;
		Sound(const Sound& other);
		Sound& operator=(const Sound& other);
		Sound(Sound&& other) noexcept;
		Sound& operator=(Sound&& other) noexcept;
		~Sound();

		Sound(const std::string &filePath);

		int32 getChannels() const;
		int32 getSampleRate() const;
		int32 getFrames() const;

		SNDFILE* getFile() const;
	};
}