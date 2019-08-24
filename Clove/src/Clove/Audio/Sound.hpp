#pragma once

#include <sndfile.h> //TODO: cpp?

namespace clv::sfx{
	class Sound{
		//friend class AudioPlayer; //Might change

		//TODO
		//Reprensts a sound - something the audio player will use to play'

		//VARIABLES
	//private:
	public: //temp public
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

		Sound(std::string filePath);
	};
}