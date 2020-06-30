#include "Bulb/ECS/Components/AudioComponent.hpp"

namespace blb::ecs {
	AudioComponent::AudioComponent() = default;

	AudioComponent::AudioComponent(const AudioComponent& other) = default;

	AudioComponent::AudioComponent(AudioComponent&& other) noexcept {
//		sound = std::move(other.sound);

		stream = other.stream;
		playbackPosition = other.playbackPosition;

		requestedPlayback = std::move(other.requestedPlayback);
		currentPlayback = std::move(other.currentPlayback);
		requestedStopMode = std::move(other.requestedStopMode);
	}

	AudioComponent& AudioComponent::operator=(const AudioComponent& other) = default;

	AudioComponent& AudioComponent::operator=(AudioComponent&& other) noexcept {
		//sound = std::move(other.sound);

		stream = other.stream;
		playbackPosition = other.playbackPosition;

		requestedPlayback = std::move(other.requestedPlayback);
		currentPlayback = std::move(other.currentPlayback);
		requestedStopMode = std::move(other.requestedStopMode);

		return *this;
	}

	AudioComponent::~AudioComponent() = default;

	void AudioComponent::setSound(aud::Sound sound) {
		//this->sound = std::move(sound);
	}

	void AudioComponent::play(PlaybackMode playback) {
		/*if(sound.isValid()) {
			requestedPlayback = playback;
		} else {
			GARLIC_LOG(garlicLogContext, clv::Log::Level::Error, "{0} called without valid sound. Please call setSound with an initialised Sound class", CLV_FUNCTION_NAME);
		}*/
	}

	void AudioComponent::pause() {
		/*if(sound.isValid()) {
			requestedStopMode = StopMode::pause;
		} else {
			GARLIC_LOG(garlicLogContext, clv::Log::Level::Error, "{0} called without valid sound. Please call setSound with an initialised Sound class", CLV_FUNCTION_NAME);
		}*/
	}

	void AudioComponent::resume() {
		/*if(sound.isValid()) {
			if(currentPlayback && !isPlaying()) {
				requestedPlayback = currentPlayback;
			}
		} else {
			GARLIC_LOG(garlicLogContext, clv::Log::Level::Error, "{0} called without valid sound. Please call setSound with an initialised Sound class", CLV_FUNCTION_NAME);
		}*/
	}

	void AudioComponent::stop() {
		/*if(sound.isValid()) {
			requestedStopMode = StopMode::stop;
		} else {
			GARLIC_LOG(garlicLogContext, clv::Log::Level::Error, "{0} called without valid sound. Please call setSound with an initialised Sound class", CLV_FUNCTION_NAME);
		}*/
	}

	bool AudioComponent::isPlaying() {
		return playing;
	}
}