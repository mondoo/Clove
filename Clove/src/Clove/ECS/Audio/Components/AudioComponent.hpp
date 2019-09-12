#pragma once

#include "Clove/ECS/Component.hpp"
#include "Clove/Audio/Sound.hpp"

#include <portaudio.h> //TODO: cpp

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
		static constexpr ComponentID ID = 0x7c10e639; //VS Generated GUID

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
		//TODO: Ctors

		void setSound(const clv::aud::Sound& sound); //TODO: make val?

		void play(PlaybackMode playback = PlaybackMode::once);
		void pause();
		void stop();

		bool isPlaying();
	};
}