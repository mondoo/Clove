#include "Clove/Components/AudioListenerComponent.hpp"

#include "Clove/Application.hpp"

#include <Clove/Audio/AudioFactory.hpp>
#include <Clove/Audio/AudioListener.hpp>

namespace garlic::clove {
    AudioListenerComponent::AudioListenerComponent() {
        listener = Application::get().getAudioFactory()->createAudioListener();
    }

    AudioListenerComponent::AudioListenerComponent(AudioListenerComponent &&other) noexcept = default;

    AudioListenerComponent &AudioListenerComponent::operator=(AudioListenerComponent &&other) noexcept = default;

    AudioListenerComponent::~AudioListenerComponent() = default;
}