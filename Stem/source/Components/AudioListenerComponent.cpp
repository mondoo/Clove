#include "Stem/Components/AudioListenerComponent.hpp"

#include "Stem/Application.hpp"

#include <Clove/Audio/AudioFactory.hpp>
#include <Clove/Audio/AudioListener.hpp>

namespace garlic::inline stem {
    AudioListenerComponent::AudioListenerComponent() {
        listener = Application::get().getAudioFactory()->createAudioListener();
    }

    AudioListenerComponent::AudioListenerComponent(AudioListenerComponent &&other) noexcept = default;

    AudioListenerComponent &AudioListenerComponent::operator=(AudioListenerComponent &&other) noexcept = default;

    AudioListenerComponent::~AudioListenerComponent() = default;
}