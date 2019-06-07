#pragma once

#include "Clove/ECS/ECSTypes.hpp"

namespace clv::ecs{
	class Entity;
	class Component;
	class BaseSystem;

	class Manager{
		//VARIABLES
	private:
		//These guys are all of everything
		std::unordered_map<EntityID, Entity*> entities; //TODO: ordered map?
		std::vector<std::vector<Component*>> components; //TODO: unique ptr --- this is every single component
		std::vector<BaseSystem*> systems; //TODO: unique_ptr?

		//FUNCTIONS
	public:
		Manager();
		Manager(const Manager& other) = delete;
		Manager(Manager&& other) noexcept = delete;
		Manager& operator=(const Manager& other) = delete;
		Manager& operator=(Manager&& other) noexcept = delete;
		~Manager();

		void update(float deltaTime);

		EntityID createEntity();
		void destroyEntity(EntityID);
	};
}
