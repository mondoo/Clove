#pragma once

#include "Clove/Audio/Sound.hpp"

#include <portaudio.h> //TODO: move to cpp
//Temp
#include "Sound.hpp"

namespace clv::aud{
	struct AudioStreamID{
		int32 ID = -1;
	};

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

		AudioStreamID playSound(/*const*/ Sound& sound);
		void stopSound(AudioStreamID ID);

	private:
		AudioStreamID generateNextID();
	};
}