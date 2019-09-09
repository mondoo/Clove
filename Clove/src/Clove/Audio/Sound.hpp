#pragma once

#include <sndfile.hh> //TODO: move to cpp???
#include <portaudio.h>

namespace clv::aud{
	struct StreamData{
		uint32 position = 0;
		SndfileHandle file;
	};

	enum class PlaybackMode{
		once,
		repeat
	};

	class Sound{
		friend class SoundPlayer;

		//VARIABLES
	private:
		PaStream* openStream = nullptr;
		SndfileHandle file;
		
		std::optional<PlaybackMode> currentPlaybackMode;
		StreamData activeStreamData;

		//FUNCTIONS
	public:
		Sound();
		Sound(const Sound& other);
		Sound& operator=(const Sound& other);
		Sound(Sound&& other);
		Sound& operator=(Sound&& other);
		~Sound();

		Sound(const std::string &filePath);

		void play(PlaybackMode playback = PlaybackMode::once);
		void pause();
		void stop();

		bool isPlaying();

	private:
		static int soundPlayback_Loop(const void* inputBuffer, void* outputBuffer, unsigned long frameCount, const PaStreamCallbackTimeInfo* timeInfo, PaStreamCallbackFlags statusFlags, void* userData);
		static int soundPlayback_Once(const void* inputBuffer, void* outputBuffer, unsigned long frameCount, const PaStreamCallbackTimeInfo* timeInfo, PaStreamCallbackFlags statusFlags, void* userData);
	};
}