#pragma once

#include "Core/ECS/Component.hpp"

#include "Core/Audio/Sound.hpp"
#include "Core/Utils/HashString.hpp"

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

	class AudioComponent : public Component{
		friend class AudioSystem;

		//VARIABLES
	public:
		static constexpr ComponentID ID = HASH_CLASS(AudioComponent);

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