#pragma once

#include "Clove/Audio/Sound.hpp"

namespace clv::aud{
	class SoundPlayer{
		//VARIABLES
	private:
		

		//FUNCTIONS
	public:
		SoundPlayer();
		SoundPlayer(const SoundPlayer& other) = delete;
		SoundPlayer& operator=(const SoundPlayer& other) = delete;
		SoundPlayer(SoundPlayer&& other) = delete;
		SoundPlayer& operator=(SoundPlayer&& other) = delete;
		~SoundPlayer();

		void playSound(/*const*/ Sound& sound);
	};
}