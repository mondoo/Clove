#pragma once

#include "Bulb/ECS/Component.hpp"

#include "Bulb/Audio/Sound.hpp"

//PortAudio type def (see portaudio.h)
typedef void PaStream;

namespace blb::ecs {
	enum class PlaybackMode {
		once,
		repeat
	};

	enum class StopMode {
		pause,
		stop
	};

	class AudioComponent : public Component<AudioComponent> {
		friend class AudioSystem;

		//VARIABLES
	private:
		aud::Sound sound;

		PaStream* stream = nullptr;
		uint32_t playbackPosition = 0u;

		std::optional<PlaybackMode> requestedPlayback;
		std::optional<PlaybackMode> currentPlayback;
		std::optional<StopMode> requestedStopMode;

		bool playing = false;

		//FUNCTIONS
	public:
		AudioComponent();

		AudioComponent(const AudioComponent& other);
		AudioComponent(AudioComponent&& other);

		AudioComponent& operator=(const AudioComponent& other);
		AudioComponent& operator=(AudioComponent&& other);

		~AudioComponent();

		void setSound(aud::Sound sound);

		void play(PlaybackMode playback = PlaybackMode::once);
		void pause();
		void resume();
		void stop();

		bool isPlaying();
	};
}