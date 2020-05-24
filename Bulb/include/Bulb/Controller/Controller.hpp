#pragma once

#include "Bulb/ECS/Entity.hpp"

namespace blb {
	class Controller {
		//FUNCTIONS
	public:
		virtual ~Controller() = default;

		virtual void controll(blb::ecs::Entity entity) = 0;
	};
}