#pragma once

#include <sndfile.hh> //TODO: move to cpp???

namespace blb::aud {
	class Sound {
		friend class SoundPlayer;

		//VARIABLES
	private:
		//TODO: Rewrite this to use the raw ptr? just to remove the header dependency
		SndfileHandle file;

		//FUNCTIONS
	public:
		Sound();
		Sound(std::string_view filePath);

		Sound(const Sound& other);
		Sound(Sound&& other);

		Sound& operator=(const Sound& other);
		Sound& operator=(Sound&& other);

		~Sound();

		bool isValid() const;

		//TODO: Rename / retype these wrappers
		sf_count_t seek(sf_count_t frames, int whence);
		sf_count_t readf(int* ptr, sf_count_t frames);

		int32_t getChannels() const;
		int32_t getSamplerate() const;
		int32_t getFrames() const;
	};
}