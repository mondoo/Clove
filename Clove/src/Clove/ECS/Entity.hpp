#pragma once

#include "Clove/ECS/ECSTypes.hpp"

namespace clv::ecs{
	class Manager;

	class Entity{
		//VARIABLES
	private:
		Manager* manager = nullptr;
		EntityID entityID = INVALID_ENTITY_ID;

		//FUNCTIONS
	public:
		Entity();
		Entity(const Entity& other);
		Entity(Entity&& other) noexcept;
		Entity& operator=(const Entity& other);
		Entity& operator=(Entity&& other) noexcept;
		~Entity();

		Entity(Manager* manager, EntityID entityID);

		bool isValid() const;

		EntityID getID();

		template<typename ComponentType>
		ComponentType* getComponent() const;
	};
}

#include "Entity.inl"