#pragma once

#include "Clove/ECS/ECSTypes.hpp"
#include "Clove/Utils/Delegate.hpp"

namespace clv::ecs{
	class Component;

	class Entity{
		//VARIABLES
	public:
		utl::SingleCastDelegate<Component*(EntityID, ComponentID)> onComponentRequestedDelegate;
		utl::SingleCastDelegate<bool(EntityID)> isEntityIdValidDelegate;

	private:
		EntityID entityID = INVALID_ENTITY_ID;

		//FUNCTIONS
	public:
		Entity();
		Entity(const Entity& other);
		Entity(Entity&& other) noexcept;
		Entity& operator=(const Entity& other);
		Entity& operator=(Entity&& other) noexcept;
		~Entity();

		Entity(EntityID entityID);

		bool isValid() const;

		EntityID getID() const;

		template<typename ComponentType>
		ComponentType* getComponent() const;
	};
}

#include "Entity.inl"