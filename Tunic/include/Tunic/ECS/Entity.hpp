#pragma once

#include "Tunic/ECS/ECSTypes.hpp"
#include "Tunic/ECS/Component.hpp"

namespace tnc::ecs{
	class World;
}

namespace tnc::ecs{
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