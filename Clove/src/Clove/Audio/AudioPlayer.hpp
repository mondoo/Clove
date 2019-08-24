#pragma once

#include "Clove/Audio/Sound.hpp"

namespace clv::sfx{
	class AudioPlayer{
		//VARIABLES
	private:
		//Keep track of open streams?

		//FUNCTIONS
	public:
		AudioPlayer();
		AudioPlayer(const AudioPlayer& other) = delete;
		AudioPlayer& operator=(const AudioPlayer& other) = delete;
		AudioPlayer(AudioPlayer&& other) = delete;
		AudioPlayer& operator=(AudioPlayer&& other) = delete;
		~AudioPlayer();

		void playSound(/*const*/ Sound& sound);
	};
}