#include "Bulb/Audio/Sound.hpp"

namespace blb::aud {
	Sound::Sound() = default;

	Sound::Sound(std::string_view filePath)
		: file(filePath.data()) {
	}

	Sound::Sound(const Sound& other) = default;

	Sound::Sound(Sound&& other) = default;

	Sound& Sound::operator=(const Sound& other) = default;

	Sound& Sound::operator=(Sound&& other) = default;

	Sound::~Sound() = default;

	bool Sound::isValid() const {
		return file.refCount() > 0;
	}

	sf_count_t Sound::seek(sf_count_t frames, int whence) {
		return file.seek(frames, whence);
	}

	sf_count_t Sound::readf(int* ptr, sf_count_t frames) {
		return file.readf(ptr, frames);
	}

	int32_t Sound::getChannels() const {
		return file.channels();
	}

	int32_t Sound::getSamplerate() const {
		return file.samplerate();
	}

	int32_t Sound::getFrames() const {
		return file.frames();
	}
}