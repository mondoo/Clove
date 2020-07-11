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
}