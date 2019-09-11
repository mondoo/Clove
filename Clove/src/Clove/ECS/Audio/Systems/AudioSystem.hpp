#pragma once

#include "Clove/ECS/System.hpp"
#include "Clove/ECS/Audio/Components/AudioComponent.hpp"

//TODO: cpp
#include <portaudio.h>

namespace clv::ecs::aud{
	class AudioSystem : public System<AudioComponent>{
		//VARIABLES
	private:

		//FUNCTIONS
	public:
		//TODO: others
		AudioSystem();
		~AudioSystem();

		virtual void update(utl::DeltaTime deltaTime) override;

	private:
		void startStream(AudioComponent* component, PlaybackMode playback);
		void pauseStream(PaStream* stream);
		void stopStream(PaStream* stream);

		virtual void handleEntityDestruction(const ComponentTuple& componentTuple) override;

		static int soundPlayback_Loop(const void* inputBuffer, void* outputBuffer, unsigned long frameCount, const PaStreamCallbackTimeInfo* timeInfo, PaStreamCallbackFlags statusFlags, void* userData);
		static int soundPlayback_Once(const void* inputBuffer, void* outputBuffer, unsigned long frameCount, const PaStreamCallbackTimeInfo* timeInfo, PaStreamCallbackFlags statusFlags, void* userData);
	};
}