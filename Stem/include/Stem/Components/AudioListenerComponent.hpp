#pragma once

#include <Bulb/ECS/Component.hpp>

namespace clv {
    class AudioFactory;
    class AudioListener;
}

namespace garlic::inline stem {
    /**
	 * @brief Allows an entity to determine where a sound comes from
	 * @details Every ecs::World can only have 1 AudioListenerComponent
	 */
    class AudioListenerComponent : public blb::ecs::Component<AudioListenerComponent> {
        friend class AudioSystem;

        //VARIABLES
    private:
        std::unique_ptr<clv::AudioListener> listener;

        //FUNCTIONS
    public:
        AudioListenerComponent() = delete;
        AudioListenerComponent(clv::AudioFactory &factory);

        AudioListenerComponent(AudioListenerComponent const &other) = delete;
        AudioListenerComponent(AudioListenerComponent &&other) noexcept;

        AudioListenerComponent &operator=(AudioListenerComponent const &other) = delete;
        AudioListenerComponent &operator=(AudioListenerComponent &&other) noexcept;

        ~AudioListenerComponent();
    };
}
