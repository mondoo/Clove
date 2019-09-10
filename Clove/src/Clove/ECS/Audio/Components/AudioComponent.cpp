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

	}

	void AudioComponent::stop(){
		//TODO error if no sound file attached

	}

	bool AudioComponent::isPlaying(){
		return playing;
	}
}