#pragma once

#include "Bulb/Controller/Controller.hpp"

namespace blb {
	class AIController : Controller {
		//VARIABLES
	private:
		//TODO: Takes in a behaviour tree and executes that on the entity

		//FUNCTIONS
	public:
		void controll(blb::ecs::Entity entity) override;
	};
}