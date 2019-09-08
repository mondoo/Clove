#pragma once

#include "Clove/ECS/System.hpp"
#include "Clove/ECS/Audio/Components/AudioComponent.hpp"

namespace clv::ecs::aud{
	class AudioSystem : public System<AudioComponent>{
		//VARIABLES
	private:

		//FUNCTIONS
	public:
		virtual void update(utl::DeltaTime deltaTime) override;
	};
}