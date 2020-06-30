#include "Bulb/Audio/Sound.hpp"

#include <Clove/Audio/AudioBuffer.hpp>
#include <Clove/Audio/AudioFactory.hpp>
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
    static clv::BufferFormat getBufferFormat(const SF_INFO& fileInfo) {
        const bool is16 = (fileInfo.format & SF_FORMAT_PCM_16) != 0;

        if(fileInfo.channels == 1) {
            return is16 ? clv::BufferFormat::Mono16 : clv::BufferFormat::Mono8;
        } else {
            return is16 ? clv::BufferFormat::Stereo16 : clv::BufferFormat::Stereo8;
        }
    }

    Sound::Sound() = default;

    Sound::Sound(clv::AudioFactory& factory, std::string_view filePath)
        : data(std::make_shared<SoundData>(filePath)) {
        const size_t bufferSize = data->fileInfo.channels * data->fileInfo.frames;
        short* rawBuffer        = new short[bufferSize];
        sf_readf_short(data->file, rawBuffer, data->fileInfo.frames);

        clv::AudioBufferDescriptor descriptor{};
        descriptor.format     = getBufferFormat(data->fileInfo);
        descriptor.sampleRate = data->fileInfo.samplerate;
        buffer                = factory.createAudioBuffer(std::move(descriptor), rawBuffer, bufferSize * sizeof(short));

        delete[] rawBuffer;
    }

    Sound::Sound(Sound&& other) noexcept = default;

    Sound& Sound::operator=(Sound&& other) noexcept = default;

    Sound::~Sound() = default;
}