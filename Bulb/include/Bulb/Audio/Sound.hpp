#pragma once

namespace blb::aud {
    struct SoundData;
}

namespace blb::aud {
    class Sound {
        //VARIABLES
    private:
        std::shared_ptr<SoundData> data;

        //FUNCTIONS
    public:
        Sound();
        Sound(std::string_view filePath);

        Sound(const Sound& other);
        Sound(Sound&& other);

        Sound& operator=(const Sound& other);
        Sound& operator=(Sound&& other);

        ~Sound();
    };
}