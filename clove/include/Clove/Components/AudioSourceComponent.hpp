#pragma once

#include <Clove/ECS/Component.hpp>

#include <vector>
#include <memory>

namespace garlic::clove {
    class AudioBuffer;
    class AudioFactory;
    class AudioSource;

    enum class PlaybackMode {
        Once,
        Repeat
    };
}

namespace garlic::clove {
    /**
	 * @brief Allows an entity to determine where a sound is coming from
	 */
    class AudioSourceComponent : public Component<AudioSourceComponent> {
        friend class AudioSystem;

        //VARIABLES
    private:
        std::unique_ptr<AudioSource> source;

        //FUNCTIONS
    public:
        AudioSourceComponent();

        AudioSourceComponent(AudioSourceComponent const &other) = delete;
        AudioSourceComponent(AudioSourceComponent &&other) noexcept;

        AudioSourceComponent &operator=(AudioSourceComponent const &other) = delete;
        AudioSourceComponent &operator=(AudioSourceComponent &&other) noexcept;

        ~AudioSourceComponent();

        void setBuffer(std::shared_ptr<AudioBuffer> buffer);
        void queueBuffers(std::vector<std::shared_ptr<AudioBuffer>> buffers);
        std::vector<std::shared_ptr<AudioBuffer>> unQueueBuffers(uint32_t const numToUnQueue);

        void play(PlaybackMode playback = PlaybackMode::Once);
        void pause();
        void resume();
        void stop();
    };
}
