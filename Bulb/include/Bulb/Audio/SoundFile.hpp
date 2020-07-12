#pragma once

namespace blb::aud {
	/**
	 * @brief Opens a sound file (.wav etc.) and allows acess to the data
	 */
    class SoundFile {
        //TYPES
    private:
        struct FileData;

    public:
        enum class Format {
            Unknown,
            S8, /**< signed 8 bit data */
            S16,/**< signed 16 bit data */
            S24,/**< signed 24 bit data */
            S32,/**< signed 32 bit data */
        };

        enum class SeekPosition{
            Beginning, /**< Use the beginning of the file */
            Current, /**< Use the current posititon of the cursor */
            End /**< Use the end of the file */
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

        /**
		 * @brief Reads the amount of frames and returns a pointer to that data
		 * @details Reads from the current seek position (default start of file) and
		 * moves the cursor frames amount long.
		 * @param frames How many frames to read
		 * @returns Returns an std::pair containing a pointer to the data and how many btyes long the buffer is
		 */
		std::pair<short*, size_t> read(const uint32_t frames);

        /**
		 * @brief Moves the internal cursor.
		 * @details The next read called after calling seek will cause the data to
		 * be read from that position.
		 *@param position Where to start the seek from
		 *@param frames How many frames from position to put the cursor
		 */
		void seek(SeekPosition position, uint32_t frames = 0);
    };
}
