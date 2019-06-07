#pragma once

#include "Clove/ECS/ECSTypes.hpp"

namespace clv::ecs{
	class Entity;
	class System;

	class Manager{
		//VARIABLES
	private:
		std::unordered_map<EntityID, Entity*> entities; //TODO: ordered map?
		//Store components?
		std::vector<System*> systems; //TODO: unique_ptr?

		//FUNCTIONS
	public:
	};
}
