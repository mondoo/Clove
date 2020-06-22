#pragma once

#include "Bulb/ECS/Entity.hpp"

namespace blb {
	/**
	 * @brief A Controller allows specific actions to effect an Entity
	 *
	 * @see BTAIController
	 * @see PlayerController
	 */
	class Controller {
		//FUNCTIONS
	public:
		virtual ~Controller() = default;

		virtual void possess(blb::ecs::Entity entity) = 0;
	};
}
