#pragma once

#include "Bulb/ECS/Component.hpp"

namespace clv {
    class AudioBuffer;
    class AudioFactory;
    class AudioSource;
}

namespace blb::ecs {
    enum class PlaybackMode {
        Once,
        Repeat
    };
}

namespace blb::ecs {
    class AudioSourceComponent : public Component<AudioSourceComponent> {
        friend class AudioSystem;

        //VARIABLES
    private:
        std::shared_ptr<clv::AudioBuffer> buffer;
        std::unique_ptr<clv::AudioSource> source;

        bool playing = false;

        //FUNCTIONS
    public:
        AudioSourceComponent() = delete;
        AudioSourceComponent(clv::AudioFactory& factory, std::string_view filePath);

        AudioSourceComponent(const AudioSourceComponent& other) = delete;
        AudioSourceComponent(AudioSourceComponent&& other) noexcept;

        AudioSourceComponent& operator=(const AudioSourceComponent& other) = delete;
        AudioSourceComponent& operator=(AudioSourceComponent&& other) noexcept;

        ~AudioSourceComponent();

        void play(PlaybackMode playback = PlaybackMode::Once);
        void pause();
        void resume();
        void stop();

        bool isPlaying();
    };
}