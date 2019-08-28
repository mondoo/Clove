#include "clvpch.hpp"
#include "Sound.hpp"

namespace clv::aud{
	Sound::Sound(const Sound& other) = default;

	Sound& Sound::operator=(const Sound& other) = default;

	Sound::Sound(Sound&& other) = default;
	
	Sound& Sound::operator=(Sound&& other) = default;

	Sound::~Sound() = default;
	
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

	SNDFILE* Sound::getFile(){
		return file.rawHandle();
	}
}