#pragma once

#include "Bulb/ECS/Component.hpp"

namespace clv {
    class AudioFactory;
    class AudioListener;
}

namespace blb::ecs {
    class AudioListenerComponent : public Component<AudioListenerComponent> {
        friend class AudioSystem;

        //VARIABLES
    private:
        std::unique_ptr<clv::AudioListener> listener;

        //FUNCTIONS
    public:
        AudioListenerComponent() = delete;
        AudioListenerComponent(clv::AudioFactory& factory);

        AudioListenerComponent(const AudioListenerComponent& other) = delete;
        AudioListenerComponent(AudioListenerComponent&& other) noexcept;

        AudioListenerComponent& operator=(const AudioListenerComponent& other) = delete;
        AudioListenerComponent& operator=(AudioListenerComponent&& other) noexcept;

        ~AudioListenerComponent();
    };
}