#pragma once

#include "Clove/Audio/SoundTypes.hpp"

#include <sndfile.hh> //TODO: move to cpp???

namespace clv::aud{
	class Sound{
		friend class SoundPlayer;

		//VARIABLES
	private:
		SndfileHandle file;
		int32 position = 0;
		AudioStreamID streamID;

		//FUNCTIONS
	public:
		Sound() = default; //Temp default;
		Sound(const Sound& other);
		Sound& operator=(const Sound& other);
		Sound(Sound&& other);
		Sound& operator=(Sound&& other);
		~Sound();

		Sound(const std::string &filePath);

		int32 getChannels() const;
		int32 getSampleRate() const;
		int32 getFrames() const;
	};
}