#include "Bulb/Audio/Sound.hpp"

#include <sndfile.h>

namespace blb::aud {
    struct SoundData {
        SNDFILE* file;
        SF_INFO fileInfo;

        SoundData(std::string_view path) {
            file = sf_open(path.data(), SFM_READ, &fileInfo);
        }

        ~SoundData() {
            sf_close(file);
        }
    };
}

namespace blb::aud {
    Sound::Sound() = default;

    Sound::Sound(std::string_view filePath)
        : data(std::make_shared<SoundData>(filePath)){
    }

    Sound::Sound(const Sound& other) = default;

    Sound::Sound(Sound&& other) = default;

    Sound& Sound::operator=(const Sound& other) = default;

    Sound& Sound::operator=(Sound&& other) = default;

    Sound::~Sound() = default;
}