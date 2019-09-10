#include "clvpch.hpp"
#include "Sound.hpp"

namespace clv::aud{
	Sound::Sound(){
		//Still needed?
	}

	Sound::Sound(const Sound& other) = default;

	Sound& Sound::operator=(const Sound& other) = default;
	Sound::Sound(Sound&& other) = default;

	Sound& Sound::operator=(Sound&& other) = default;

	Sound::~Sound(){
		/*if(isPlaying()){
			CLV_LOG_WARN("Sound object was deconstructed while playing... stopping sound");
			stop();
		}*/
	}

	Sound::Sound(const std::string& filePath)
		: file(filePath.c_str()){
	}

	sf_count_t Sound::seek(sf_count_t frames, int whence){
		return file.seek(frames, whence);
	}

	sf_count_t Sound::readf(int* ptr, sf_count_t frames){
		return file.readf(ptr, frames);
	}

	int32 Sound::getChannels(){
		return file.channels();
	}

	int32 Sound::getSamplerate(){
		return file.samplerate();
	}

	int32 Sound::getFrames(){
		return file.frames();
	}
}