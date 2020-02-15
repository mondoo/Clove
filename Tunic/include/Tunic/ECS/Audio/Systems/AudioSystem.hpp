#pragma once

#include "Tunic/ECS/Core/System.hpp"

#include "Tunic/ECS/Audio/Components/AudioComponent.hpp"

//PortAudio type defas (see portaudio.h)
typedef void PaStream;
typedef unsigned long PaStreamCallbackFlags;

struct PaStreamCallbackTimeInfo;

//namespace tnc::ecs::aud{
//	class AudioComponent;
//}

namespace tnc::ecs::aud{
	class AudioSystem : public System{
		//VARIABLES
	private:

		//FUNCTIONS
	public:
		AudioSystem();

		AudioSystem(const AudioSystem& other) = delete;
		AudioSystem(AudioSystem&& other) noexcept;

		AudioSystem& operator=(const AudioSystem& other) = delete;
		AudioSystem& operator=(AudioSystem&& other) noexcept;

		~AudioSystem();

		virtual void update(World& world, clv::utl::DeltaTime deltaTime) override;

		virtual void onComponentDestroyed(ComponentInterface* component) override;

	private:
		void startSound(AudioComponent* component, PlaybackMode playback);
		void pauseSound(AudioComponent* component);
		void stopSound(AudioComponent* component);

		bool isStreamActive(PaStream* stream);

		static int soundPlayback_Loop(const void* inputBuffer, void* outputBuffer, unsigned long frameCount, const PaStreamCallbackTimeInfo* timeInfo, PaStreamCallbackFlags statusFlags, void* userData);
		static int soundPlayback_Once(const void* inputBuffer, void* outputBuffer, unsigned long frameCount, const PaStreamCallbackTimeInfo* timeInfo, PaStreamCallbackFlags statusFlags, void* userData);
	};
}