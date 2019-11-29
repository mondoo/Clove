#pragma once

#include "Clove/Core/ECS/Component.hpp"

#include "Clove/Core/Audio/Sound.hpp"

//PortAudio type def (see portaudio.h)
typedef void PaStream;

namespace clv::ecs::aud{
	enum class PlaybackMode{
		once,
		repeat
	};

	enum class StopMode{
		pause,
		stop
	};

	class AudioComponent : public Component<AudioComponent>{
		friend class AudioSystem;

		//VARIABLES
	private:
		clv::aud::Sound sound;

		PaStream* stream = nullptr;
		uint32 playbackPosition = 0;

		std::optional<PlaybackMode> requestedPlayback;
		std::optional<PlaybackMode> currentPlayback;
		std::optional<StopMode> requestedStopMode;

		bool playing = false;

		//FUNCTIONS
	public:
		AudioComponent();
		AudioComponent(const AudioComponent& other) = delete;
		AudioComponent(AudioComponent&& other);
		AudioComponent& operator=(const AudioComponent& other) = delete;
		AudioComponent& operator=(AudioComponent&& other);
		virtual ~AudioComponent();

		void setSound(const clv::aud::Sound& sound); //TODO: make val?

		void play(PlaybackMode playback = PlaybackMode::once);
		void pause();
		void resume();
		void stop();

		bool isPlaying();
	};
}