#include "Clove/Components/AudioListenerComponent.hpp"

#include "Clove/Application.hpp"

#include <Clove/Audio/AudioDevice.hpp>
#include <Clove/Audio/AudioFactory.hpp>
#include <Clove/Audio/AudioListener.hpp>

namespace garlic::clove {
    AudioListenerComponent::AudioListenerComponent() {
        listener = *Application::get().getAudioDevice()->getAudioFactory()->createAudioListener();
    }

    AudioListenerComponent::AudioListenerComponent(AudioListenerComponent &&other) noexcept = default;

    AudioListenerComponent &AudioListenerComponent::operator=(AudioListenerComponent &&other) noexcept = default;

    AudioListenerComponent::~AudioListenerComponent() = default;
}