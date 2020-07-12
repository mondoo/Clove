#include "Bulb/Audio/SoundFile.hpp"

#include <sndfile.h>

namespace blb::aud {
    struct SoundFile::FileData {
        SNDFILE* file;
        SF_INFO fileInfo;
    };

    SoundFile::SoundFile(std::string_view file) {
        data = std::make_unique<FileData>();

        data->file = sf_open(file.data(), SFM_READ, &data->fileInfo);
    }

    SoundFile::SoundFile(SoundFile&& other) noexcept = default;

    SoundFile& SoundFile::operator=(SoundFile&& other) noexcept = default;

    SoundFile::~SoundFile() {
        sf_close(data->file);
    }

    uint32_t SoundFile::getChannels() const {
        return data->fileInfo.channels;
    }

    uint32_t SoundFile::getFrames() const {
        return data->fileInfo.frames;
    }

    uint32_t SoundFile::getSampleRate() const {
        return data->fileInfo.samplerate;
    }

    SoundFile::Format SoundFile::getFormat() const {
        if((data->fileInfo.format & SF_FORMAT_PCM_S8) != 0) {
            return Format::s8;
        } else if((data->fileInfo.format & SF_FORMAT_PCM_16) != 0) {
            return Format::s16;
        } else if((data->fileInfo.format & SF_FORMAT_PCM_24) != 0) {
            return Format::s24;
        } else if((data->fileInfo.format & SF_FORMAT_PCM_32) != 0) {
            return Format::s32;
        }

        CLV_ASSERT(false, "{0}, Unknown file format", CLV_FUNCTION_NAME_PRETTY);
        return Format::unknown;
    }
}