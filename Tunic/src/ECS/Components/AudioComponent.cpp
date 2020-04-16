#include "Tunic/ECS/Components/AudioComponent.hpp"

namespace tnc::ecs {
	AudioComponent::AudioComponent() = default;

	AudioComponent::AudioComponent(const AudioComponent& other) = default;

	AudioComponent::AudioComponent(AudioComponent&& other) = default;

	AudioComponent& AudioComponent::operator=(const AudioComponent& other) = default;

	AudioComponent& AudioComponent::operator=(AudioComponent&& other) = default;

	AudioComponent::~AudioComponent() = default;

	void AudioComponent::setSound(const aud::Sound& sound) {
		this->sound = sound;
	}

	void AudioComponent::play(PlaybackMode playback) {
		if(sound.isValid()) {
			requestedPlayback = playback;
		} else {
			CLV_LOG_ERROR("{0} called without valid sound. Please call setSound with an initialised Sound class", CLV_FUNCTION_NAME);
		}
	}

	void AudioComponent::pause() {
		if(sound.isValid()) {
			requestedStopMode = StopMode::pause;
		} else {
			CLV_LOG_ERROR("{0} called without valid sound. Please call setSound with an initialised Sound class", CLV_FUNCTION_NAME);
		}
	}

	void AudioComponent::resume() {
		if(sound.isValid()) {
			if(currentPlayback && !isPlaying()) {
				requestedPlayback = currentPlayback;
			}
		} else {
			CLV_LOG_ERROR("{0} called without valid sound. Please call setSound with an initialised Sound class", CLV_FUNCTION_NAME);
		}
	}

	void AudioComponent::stop() {
		if(sound.isValid()) {
			requestedStopMode = StopMode::stop;
		} else {
			CLV_LOG_ERROR("{0} called without valid sound. Please call setSound with an initialised Sound class", CLV_FUNCTION_NAME);
		}
	}

	bool AudioComponent::isPlaying() {
		return playing;
	}
}