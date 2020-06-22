#pragma once

#include "Bulb/ECS/ECSTypes.hpp"
#include "Bulb/ECS/Component.hpp"

namespace blb::ecs{
	class World;
}

namespace blb::ecs{
	/**
	 * A wrapper class for an EntityId
	 * This class wraps an EntityId and a pointer to the World that owns it. Allowing
	 * for a convenient way to access the Entity
	 */
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

		/**
		 * Checks if this Entity is valid
		 * An Entity will be valid if it has a valid World pointer and has a valid EntityId
		 * @see World::isEntityValid
		 * @return Returns true of the Entity is valid
		 */
		bool isValid() const;

		/**
		 * Gets the underlying EntityId this class wraps
		 * @return The EntityID this class wraps
		 */
		EntityID getID() const;

		/**
		 * Clones this Entity's components
		 * Creates a new Entity with the same Components as this one
		 * @see World::cloneEntitiesComponents
		 * @return A new Entity with the same Component layout
		 */
		Entity clone();
		/**
		 * Destroys this Entity, removing it from the World
		 * Calls World::destroyEntity
		 * @see World::destroyEntity
		 */
		void destroy();

		/**
		 * Adds a new Component to this Entity
		 * @see World::addComponent
		 * @tparam ComponentType The type of Component to add
		 * @param args Construction arguments forwarded to the Component
		 * @return A ComponentPtr to the added Component
		 * @see ComponentPtr
		 */
		template<typename ComponentType, typename ...ConstructorArgs>
		ComponentPtr<ComponentType> addComponent(ConstructorArgs&& ...args);
		/**
		 * Gets a Component from this Entity
		 * @see World::getComponent
		 * @tparam ComponentType The type of Component to get
		 * @return A ComponentPtr to the component if it exists
		 * @see ComponentPtr
		 */
		template<typename ComponentType>
		ComponentPtr<ComponentType> getComponent() const;
		/**
		 * Checks to see if this Entity has a Component
		 * @see World::hasComponent
		 * @tparam ComponentType The type of Component to check for
		 * @return Returns true if the component exists
		 */
		template<typename ComponentType>
		bool hasComponent() const;
		/**
		 * Removes a component from this Entity
		 * @see World::removeComponent
		 * @tparam The type of Component to remove
		 */
		template<typename ComponentType>
		void removeComponent();

		operator EntityID() const;
	};
}

#include "Entity.inl"
