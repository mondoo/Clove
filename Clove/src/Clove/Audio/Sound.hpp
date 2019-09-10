#pragma once

#include <sndfile.hh> //TODO: move to cpp???

namespace clv::aud{
	/*struct StreamData{
		uint32 position = 0;
		SndfileHandle file;
	};*/

	class Sound{
		friend class SoundPlayer;

		//VARIABLES
	private:
		//PaStream* openStream = nullptr;
		SndfileHandle file;

		//StreamData activeStreamData;

		//FUNCTIONS
	public:
		Sound();
		Sound(const Sound& other);
		Sound& operator=(const Sound& other);
		Sound(Sound&& other);
		Sound& operator=(Sound&& other);
		~Sound();

		Sound(const std::string& filePath);

		//TODO: Rename / retype these wrappers
		sf_count_t seek(sf_count_t frames, int whence);
		sf_count_t readf(int* ptr, sf_count_t frames);

		int32 getChannels();
		int32 getSamplerate();
		int32 getFrames();
	};
}