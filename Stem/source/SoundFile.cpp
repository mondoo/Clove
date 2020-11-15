#include "Stem/SoundFile.hpp"

#include <Root/Definitions.hpp>
#include <Root/Log/Log.hpp>
#include <sndfile.h>

namespace garlic::inline stem {
    static int getWhence(SoundFile::SeekPosition position) {
        switch(position) {
            case SoundFile::SeekPosition::Beginning:
                return SEEK_SET;
            case SoundFile::SeekPosition::Current:
                return SEEK_CUR;
            case SoundFile::SeekPosition::End:
                return SEEK_END;
            default:
                GARLIC_ASSERT(false, "{0}: Default statement hit", GARLIC_FUNCTION_NAME);
                return 0;
        }
    }

    struct SoundFile::FileData {
        SNDFILE *file;
        SF_INFO fileInfo;
    };

    SoundFile::SoundFile(std::string_view file) {
        data = std::make_unique<FileData>();

        data->file = sf_open(file.data(), SFM_READ, &data->fileInfo);
    }

    SoundFile::SoundFile(SoundFile &&other) noexcept = default;

    SoundFile &SoundFile::operator=(SoundFile &&other) noexcept = default;

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
            return Format::S8;
        } else if((data->fileInfo.format & SF_FORMAT_PCM_16) != 0) {
            return Format::S16;
        } else if((data->fileInfo.format & SF_FORMAT_PCM_24) != 0) {
            return Format::S24;
        } else if((data->fileInfo.format & SF_FORMAT_PCM_32) != 0) {
            return Format::S32;
        }

        GARLIC_ASSERT(false, "{0}, Unknown file format", GARLIC_FUNCTION_NAME_PRETTY);
        return Format::Unknown;
    }

    std::pair<short *, size_t> SoundFile::read(const uint32_t frames) {
        size_t const elementCount = frames * getChannels();
        size_t const bufferSize   = elementCount * sizeof(short);
        short *buffer             = reinterpret_cast<short *>(malloc(bufferSize));

        sf_readf_short(data->file, buffer, frames);

        return { buffer, bufferSize };
    }

    void SoundFile::seek(SeekPosition position, uint32_t frames) {
        sf_seek(data->file, frames, getWhence(position));
    }
}
