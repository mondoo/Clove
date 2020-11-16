#pragma once

#include <Bulb/ECS/Component.hpp>

namespace clv {
    class AudioBuffer;
    class AudioFactory;
    class AudioSource;
}

namespace garlic::inline stem {
    enum class PlaybackMode {
        Once,
        Repeat
    };
}

namespace garlic::inline stem {
    /**
	 * @brief Allows an entity to determine where a sound is coming from
	 */
    class AudioSourceComponent : public blb::ecs::Component<AudioSourceComponent> {
        friend class AudioSystem;

        //VARIABLES
    private:
        std::unique_ptr<clv::AudioSource> source;

        //FUNCTIONS
    public:
        AudioSourceComponent();

        AudioSourceComponent(AudioSourceComponent const &other) = delete;
        AudioSourceComponent(AudioSourceComponent &&other) noexcept;

        AudioSourceComponent &operator=(AudioSourceComponent const &other) = delete;
        AudioSourceComponent &operator=(AudioSourceComponent &&other) noexcept;

        ~AudioSourceComponent();

        void setBuffer(std::shared_ptr<clv::AudioBuffer> buffer);
        void queueBuffers(std::vector<std::shared_ptr<clv::AudioBuffer>> buffers);
        std::vector<std::shared_ptr<clv::AudioBuffer>> unQueueBuffers(uint32_t const numToUnQueue);

        void play(PlaybackMode playback = PlaybackMode::Once);
        void pause();
        void resume();
        void stop();
    };
}
