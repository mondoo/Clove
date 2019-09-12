#include "clvpch.hpp"
#include "AudioComponent.hpp"

namespace clv::ecs::aud{
	void AudioComponent::setSound(const clv::aud::Sound& sound){
		this->sound = sound;
	}

	void AudioComponent::play(PlaybackMode playback){
		//TODO error if no sound file attached
		currentPlaybackMode = playback;
	}

	void AudioComponent::pause(){
		//TODO error if no sound file attached
		requestedState = StateRequest::pause;
	}

	void AudioComponent::stop(){
		//TODO error if no sound file attached
		requestedState = StateRequest::stop;
	}

	bool AudioComponent::isPlaying(){
		return playing;
	}
}