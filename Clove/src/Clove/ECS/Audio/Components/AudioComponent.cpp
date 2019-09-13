#include "clvpch.hpp"
#include "AudioComponent.hpp"

#include <portaudio.h>

namespace clv::ecs::aud{
	void AudioComponent::setSound(const clv::aud::Sound& sound){
		this->sound = sound;
	}

	void AudioComponent::play(PlaybackMode playback){
		//TODO error if no sound file attached
		requestedPlayback = playback;
	}

	void AudioComponent::pause(){
		//TODO error if no sound file attached
		requestedStopMode = StopMode::pause;
	}

	void AudioComponent::resume(){
		//TODO error if no sound file attached
		if(currentPlayback && !isPlaying()){
			requestedPlayback = currentPlayback;
		}
	}

	void AudioComponent::stop(){
		//TODO error if no sound file attached
		requestedStopMode = StopMode::stop;
	}

	bool AudioComponent::isPlaying(){
		return playing;
	}
}