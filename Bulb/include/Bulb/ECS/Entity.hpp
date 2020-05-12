#pragma once

#include "Bulb/ECS/ECSTypes.hpp"
#include "Bulb/ECS/Component.hpp"

namespace blb::ecs{
	class World;
}

namespace blb::ecs{
	class Entity{
		//VARIABLES
	private:
		World* world = nullptr;

		EntityID entityID = INVALID_ENTITY_ID;

		//FUNCTIONS
	public:
		Entity();
		Entity(EntityID entityID, World* world);

		Entity(const Entity& other);
		Entity(Entity&& other) noexcept;

		Entity& operator=(const Entity& other);
		Entity& operator=(Entity&& other) noexcept;

		~Entity();

		bool isValid() const;

		EntityID getID() const;

		Entity clone();
		void destroy();

		template<typename ComponentType, typename ...ConstructorArgs>
		ComponentPtr<ComponentType> addComponent(ConstructorArgs&& ...args);
		template<typename ComponentType>
		ComponentPtr<ComponentType> getComponent() const;
		template<typename ComponentType>
		void removeComponent();

		operator EntityID() const;
	};
}

#include "Entity.inl"