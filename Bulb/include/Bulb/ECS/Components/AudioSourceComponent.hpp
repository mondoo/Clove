#pragma once

#include "Bulb/ECS/Component.hpp"

//#include "Bulb/Audio/Sound.hpp"

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

	class AudioSourceComponent : public Component<AudioSourceComponent> {
		friend class AudioSystem;

		//VARIABLES
	private:
		//aud::Sound sound;

		PaStream* stream = nullptr;
		uint32_t playbackPosition = 0u;

		std::optional<PlaybackMode> requestedPlayback;
		std::optional<PlaybackMode> currentPlayback;
		std::optional<StopMode> requestedStopMode;

		bool playing = false;

		//FUNCTIONS
	public:
        AudioSourceComponent();

        AudioSourceComponent(const AudioSourceComponent& other);
        AudioSourceComponent(AudioSourceComponent&& other) noexcept;

        AudioSourceComponent& operator=(const AudioSourceComponent& other);
        AudioSourceComponent& operator=(AudioSourceComponent&& other) noexcept;

		~AudioSourceComponent();

		void play(PlaybackMode playback = PlaybackMode::once);
		void pause();
		void resume();
		void stop();

		bool isPlaying();
	};
}