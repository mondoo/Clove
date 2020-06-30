#pragma once

namespace clv {
    class AudioBuffer;
    class AudioFactory;
}

namespace blb::aud {
    struct SoundData;
}

namespace blb::aud {
    class Sound {
        //VARIABLES
    private:
        std::shared_ptr<SoundData> data;
        std::unique_ptr<clv::AudioBuffer> buffer;

        //FUNCTIONS
    public:
        Sound();
        Sound(clv::AudioFactory& factory, std::string_view filePath);

        Sound(const Sound& other) = delete;
        Sound(Sound&& other) noexcept;

        Sound& operator=(const Sound& other) = delete;
        Sound& operator=(Sound&& other) noexcept;

        ~Sound();
    };
}