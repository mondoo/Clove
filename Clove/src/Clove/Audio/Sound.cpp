#include "clvpch.hpp"
#include "Sound.hpp"

namespace clv::sfx{
	Sound::Sound(const Sound& other) = default;

	Sound& Sound::operator=(const Sound& other) = default;

	Sound::Sound(Sound&& other) noexcept = default;
	
	Sound& Sound::operator=(Sound&& other) noexcept = default;

	Sound::~Sound(){
		sf_close(file);
	}
	
	Sound::Sound(const std::string& filePath){
		file = sf_open(filePath.c_str(), SFM_READ, &info);
	}

	int32 Sound::getChannels() const{
		return info.channels;
	}

	int32 Sound::getSampleRate() const{
		return info.samplerate;
	}

	SNDFILE* Sound::getFile() const{
		return file;
	}
}