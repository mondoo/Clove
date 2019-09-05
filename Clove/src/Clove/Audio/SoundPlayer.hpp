#pragma once

#include "Clove/Audio/Sound.hpp"
#include "Clove/Audio/SoundTypes.hpp"

#include <portaudio.h> //TODO: move to cpp

/*
Do we need a single sound player?
can things just make a sound player when they need to play sound??

I guess there'll only be one audio system and all the components will have the sounds?
*/

namespace clv::aud{
	class SoundPlayer{
		//VARIABLES
	private:
		//TODO: int32 here, write own hashing function for the struct
		std::unordered_map<int32, PaStream*> openStreams;

		Sound theSound;

		//FUNCTIONS
	public:
		SoundPlayer();
		SoundPlayer(const SoundPlayer& other) = delete;
		SoundPlayer& operator=(const SoundPlayer& other) = delete;
		SoundPlayer(SoundPlayer&& other) = delete;
		SoundPlayer& operator=(SoundPlayer&& other) = delete;
		~SoundPlayer();

		void playSound(Sound& sound);
		bool isSoundPlaying(const Sound& sound);
		void stopSound(const Sound& sound);

	private:
		AudioStreamID generateNextID();

		static int soundPlayback_Loop(const void* inputBuffer, void* outputBuffer, unsigned long frameCount, const PaStreamCallbackTimeInfo* timeInfo, PaStreamCallbackFlags statusFlags, void* userData);
		static int soundPlayback_Once(const void* inputBuffer, void* outputBuffer, unsigned long frameCount, const PaStreamCallbackTimeInfo* timeInfo, PaStreamCallbackFlags statusFlags, void* userData);
	};
}