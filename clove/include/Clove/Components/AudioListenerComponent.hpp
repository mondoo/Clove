#pragma once

#include <memory>

namespace garlic::clove {
    class AudioFactory;
    class AudioListener;
}

namespace garlic::clove {
    /**
	 * @brief Allows an entity to determine where a sound comes from
	 * @details Every ecs::EntityManager can only have 1 AudioListenerComponent
	 */
    class AudioListenerComponent {
        friend class AudioLayer;

        //VARIABLES
    private:
        std::unique_ptr<AudioListener> listener;

        //FUNCTIONS
    public:
        AudioListenerComponent();

        AudioListenerComponent(AudioListenerComponent const &other) = delete;
        AudioListenerComponent(AudioListenerComponent &&other) noexcept;

        AudioListenerComponent &operator=(AudioListenerComponent const &other) = delete;
        AudioListenerComponent &operator=(AudioListenerComponent &&other) noexcept;

        ~AudioListenerComponent();
    };
}
