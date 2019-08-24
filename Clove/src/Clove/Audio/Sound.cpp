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
	
	Sound::Sound(std::string filePath){ //const ref? am i storing it?
		file = sf_open(filePath.c_str(), SFM_READ, &info);
	}
}