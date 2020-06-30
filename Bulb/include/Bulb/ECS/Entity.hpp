#pragma once

#include "Bulb/ECS/ECSTypes.hpp"
#include "Bulb/ECS/Component.hpp"

namespace blb::ecs{
	class World;
}

namespace blb::ecs{
	/**
	 * @brief A wrapper class for an EntityId.
	 *
	 * This class wraps an EntityId and a pointer to the World that owns it. Allowing
	 * for a convenient way to access the Entity.
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
		 * @brief Checks if this Entity is valid.
		 *
		 * An Entity will be valid if it has a valid World pointer and has a valid EntityId.
		 *
		 * @return Returns true of the Entity is valid.
		 * @see World::isEntityValid.
		 */
		bool isValid() const;

		/**
		 * @brief Gets the underlying EntityId this class wraps.
		 *
		 * @return The EntityID this class wraps.
		 */
		EntityID getID() const;

		/**
		 * @brief Clones this Entity's components.
		 *
		 * Creates a new Entity with the same Components as this one.
		 *
		 * @return A new Entity with the same Component layout.
		 * @see World::cloneEntitiesComponents.
		 */
		Entity clone();
		/**
		 * @brief Destroys this Entity, removing it from the World.
		 *
		 * Calls World::destroyEntity.
		 *
		 * @see World::destroyEntity.
		 */
		void destroy();

		/**
		 * @brief Adds a new Component to this Entity.
		 *
		 * @tparam ComponentType The type of Component to add.
		 * @param args Construction arguments forwarded to the Component.
		 * @return A ComponentPtr to the added Component.
		 * @see World::addComponent.
		 * @see ComponentPtr.
		 */
		template<typename ComponentType, typename ...ConstructorArgs>
		ComponentPtr<ComponentType> addComponent(ConstructorArgs&& ...args);
		/**
		 * @brief Gets a Component from this Entity.
		 *
		 * @tparam ComponentType The type of Component to get.
		 * @return A ComponentPtr to the component if it exists.
		 * @see World::getComponent
		 * @see ComponentPtr.
		 */
		template<typename ComponentType>
		ComponentPtr<ComponentType> getComponent() const;
		/**
		 * @brief Checks to see if this Entity has a Component.
		 *
		 * @tparam ComponentType The type of Component to check for.
		 * @return Returns true if the component exists.
		 * @see World::hasComponent.
		 */
		template<typename ComponentType>
		bool hasComponent() const;
		/**
		 * @brief Removes a component from this Entity.
		 *
		 * @tparam The type of Component to remove.
		 * @see World::removeComponent.
		 */
		template<typename ComponentType>
		void removeComponent();

		operator EntityID() const;
	};
}

#include "Entity.inl"
