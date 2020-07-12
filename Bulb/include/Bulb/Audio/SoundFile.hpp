#pragma once

namespace blb::aud {
    //SoundFile class opens a sound file and allows access to the data
    class SoundFile {
        //TYPES
    private:
        struct FileData;

    public:
        enum class Format {
            unknown,
            s8, //signed 8bit data
            s16,//signed 16bit data
            s24,//signed 24bit data
            s32,//signed 32bit data
        };

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

        uint32_t getChannels() const;
        uint32_t getFrames() const;
        uint32_t getSampleRate() const;
        Format getFormat() const;

        //Reads the current amount of frames into a buffer of that size (frames * num of channels). user needs to delete pointer. returns pointer and size of entire buffer in bytes
        std::pair<short*, size_t> read(const uint32_t frames);
    };
}