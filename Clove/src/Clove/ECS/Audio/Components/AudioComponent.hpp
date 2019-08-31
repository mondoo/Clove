#pragma once

#include "Clove/ECS/Component.hpp"
#include "Clove/Audio/Sound.hpp"

namespace clv::ecs::aud{
	class AudioComponent : public Component{
		friend class AudioSystem;

		//VARIABLES
	public:
		static constexpr ComponentID ID = 0x7c10e639; //VS Generated GUID

	private:
		clv::aud::Sound sound;

		//FUNCTIONS
	public:

	};
}