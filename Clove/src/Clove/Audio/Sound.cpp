#include "clvpch.hpp"
#include "Sound.hpp"

namespace clv::aud{
	Sound::Sound(const Sound& other) = default;

	Sound& Sound::operator=(const Sound& other) = default;

	Sound::Sound(Sound&& other) = default;
	
	Sound& Sound::operator=(Sound&& other) = default;

	Sound::~Sound(){
		//TODO:
		//Check if this will delete the file - if so, check if stream is active, warn user and then call back to close stream
		//Waiting on refactor on sound player to point to the file and not the sound class (or something to that effect)
	}
	
	Sound::Sound(const std::string& filePath){
		file = SndfileHandle(filePath.c_str(), SFM_READ);
	}

	int32 Sound::getChannels() const{
		return file.channels();
	}

	int32 Sound::getSampleRate() const{
		return file.samplerate();
	}

	int32 Sound::getFrames() const{
		return file.frames();
	}
}