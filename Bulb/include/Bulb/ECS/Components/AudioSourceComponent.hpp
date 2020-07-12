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
        std::unique_ptr<clv::AudioSource> source;

        //FUNCTIONS
    public:
        AudioSourceComponent() = delete;
        AudioSourceComponent(clv::AudioFactory& factory);

        AudioSourceComponent(const AudioSourceComponent& other) = delete;
        AudioSourceComponent(AudioSourceComponent&& other) noexcept;

        AudioSourceComponent& operator=(const AudioSourceComponent& other) = delete;
        AudioSourceComponent& operator=(AudioSourceComponent&& other) noexcept;

        ~AudioSourceComponent();

        void setBuffer(const clv::AudioBuffer& buffer);
        void queueBuffers(std::vector<std::shared_ptr<clv::AudioBuffer>> buffers);
        std::vector<std::shared_ptr<clv::AudioBuffer>> unQueueBuffers(const uint32_t numToUnQueue);

        void play(PlaybackMode playback = PlaybackMode::Once);
        void pause();
        void resume();
        void stop();
    };
}