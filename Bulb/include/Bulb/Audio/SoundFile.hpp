#pragma once

namespace blb::aud {
    //SoundFile class opens a sound file and allows access to the data
    class SoundFile {
        //TYPES
    private:
        struct FileData;

        //VARIABLES
    private:
        std::unique_ptr<FileData> data;
        
        //FUNCTIONS
    public:
        SoundFile() = delete;
        SoundFile(std::string_view file);

        SoundFile(const SoundFile& other) = delete;
        SoundFile(SoundFile&& other) noexcept;

        SoundFile& operator=(const SoundFile& other) = delete;
        SoundFile& operator=(SoundFile&& other) noexcept;

        ~SoundFile();
    };
}