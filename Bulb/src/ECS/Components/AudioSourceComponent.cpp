#include "Bulb/ECS/Components/AudioSourceComponent.hpp"

namespace blb::ecs {
    AudioSourceComponent::AudioSourceComponent() = default;

    AudioSourceComponent::AudioSourceComponent(const AudioSourceComponent& other) = default;

    AudioSourceComponent::AudioSourceComponent(AudioSourceComponent&& other) noexcept = default;

    AudioSourceComponent& AudioSourceComponent::operator=(const AudioSourceComponent& other) = default;

    AudioSourceComponent& AudioSourceComponent::operator=(AudioSourceComponent&& other) noexcept = default;

    AudioSourceComponent::~AudioSourceComponent() = default;

	void AudioSourceComponent::play(PlaybackMode playback) {

	}

	void AudioSourceComponent::pause() {

	}

	void AudioSourceComponent::resume() {

	}

	void AudioSourceComponent::stop() {

	}

	bool AudioSourceComponent::isPlaying() {
		return playing;
	}
}