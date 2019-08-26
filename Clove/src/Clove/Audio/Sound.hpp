#pragma once

#include <sndfile.h> //TODO: move to cpp

namespace clv::aud{
	class Sound{
		//VARIABLES
	private:
		//Again, temp
	public:
		SNDFILE* file = nullptr;
		SF_INFO info = {};
		int32 position = 0;

		//I think these should hold the stream, then they can try and close it when the deconstruct????
		//If this had access to the PA interface we could put a lot of utility into this class
		
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