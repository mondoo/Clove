#pragma once

#include "Clove/ECS/System.hpp"
#include "Clove/ECS/Audio/Components/AudioComponent.hpp"

//PortAudio type defas (see portaudio.h)
typedef void PaStream;
typedef unsigned long PaStreamCallbackFlags;

struct PaStreamCallbackTimeInfo;

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
		void startSound(AudioComponent* component, PlaybackMode playback);
		void pauseSound(AudioComponent* component);
		void stopSound(AudioComponent* component);

		bool isStreamActive(PaStream* stream);

		virtual void handleEntityDestruction(const ComponentTuple& componentTuple) override;

		static int soundPlayback_Loop(const void* inputBuffer, void* outputBuffer, unsigned long frameCount, const PaStreamCallbackTimeInfo* timeInfo, PaStreamCallbackFlags statusFlags, void* userData);
		static int soundPlayback_Once(const void* inputBuffer, void* outputBuffer, unsigned long frameCount, const PaStreamCallbackTimeInfo* timeInfo, PaStreamCallbackFlags statusFlags, void* userData);
	};
}