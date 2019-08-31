#pragma once

#include "Clove/ECS/System.hpp"
#include "Clove/ECS/Audio/Components/AudioComponent.hpp"
#include "Clove/Audio/SoundPlayer.hpp"

namespace clv::ecs::aud{
	class AudioSystem : public System<AudioComponent>{
		//VARIABLES
	private:
		clv::aud::SoundPlayer soundPlayer;

		//FUNCTIONS
	public:
		virtual void update(utl::DeltaTime deltaTime) override;
	};
}