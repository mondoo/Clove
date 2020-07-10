#include "Bulb/Audio/SoundLoader.hpp"

#include <Clove/Audio/AudioBuffer.hpp>
#include <Clove/Audio/AudioFactory.hpp>
#include <sndfile.h>

namespace blb::aud {
    static clv::AudioFormat getBufferFormat(const SF_INFO& fileInfo) {
        const bool is16 = (fileInfo.format & SF_FORMAT_PCM_16) != 0;

        if(fileInfo.channels == 1) {
            return is16 ? clv::AudioFormat::Mono16 : clv::AudioFormat::Mono8;
        } else {
            return is16 ? clv::AudioFormat::Stereo16 : clv::AudioFormat::Stereo8;
        }
    }

    std::unique_ptr<clv::AudioBuffer> SoundLoader::loadSound(clv::AudioFactory& factory, std::string_view filePath) {
        SNDFILE* file;
        SF_INFO fileInfo;

        file = sf_open(filePath.data(), SFM_READ, &fileInfo);

        const size_t bufferSize = fileInfo.channels * fileInfo.frames;
        short* rawBuffer        = new short[bufferSize];
        sf_readf_short(file, rawBuffer, fileInfo.frames);

        clv::AudioDataDescriptor descriptor{};
        descriptor.format     = getBufferFormat(fileInfo);
        descriptor.sampleRate = fileInfo.samplerate;
        descriptor.data       = rawBuffer;
        descriptor.dataSize   = bufferSize * sizeof(short);
        auto buffer           = factory.createAudioBuffer(std::move(descriptor));

        delete[] rawBuffer;

        sf_close(file);

        return buffer;
    }
}
