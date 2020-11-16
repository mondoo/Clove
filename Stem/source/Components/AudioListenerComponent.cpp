#include "Stem/Components/AudioListenerComponent.hpp"

#include <Clove/Audio/AudioFactory.hpp>
#include <Clove/Audio/AudioListener.hpp>

namespace garlic::inline stem {
    AudioListenerComponent::AudioListenerComponent(clv::AudioFactory &factory) {
        listener = factory.createAudioListener();
    }

    AudioListenerComponent::AudioListenerComponent(AudioListenerComponent &&other) noexcept = default;

    AudioListenerComponent &AudioListenerComponent::operator=(AudioListenerComponent &&other) noexcept = default;

    AudioListenerComponent::~AudioListenerComponent() = default;
}