#include "Bulb/ECS/Components/AudioListenerComponent.hpp"

#include <Clove/Audio/AudioFactory.hpp>
#include <Clove/Audio/AudioListener.hpp>

namespace blb::ecs {
    AudioListenerComponent::AudioListenerComponent(clv::AudioFactory& factory) {
        listener = factory.createAudioListener();
    }

    AudioListenerComponent::AudioListenerComponent(AudioListenerComponent&& other) noexcept = default;

    AudioListenerComponent& ecs::AudioListenerComponent::operator=(AudioListenerComponent&& other) noexcept = default;

    AudioListenerComponent::~AudioListenerComponent() = default;
}