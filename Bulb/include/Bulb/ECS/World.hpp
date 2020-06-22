#pragma once

#include "Bulb/ECS/ComponentManager.hpp"
#include "Bulb/ECS/ECSTypes.hpp"
#include "Bulb/ECS/System.hpp"

#include <Clove/Event/EventDispatcher.hpp>

namespace blb::ecs {
	class Entity;
	class System;
}

namespace blb::ecs {
	/**
	 * The ECS World that contains the Entities Systems and Components
	 * A World acts as a container that stores all of the elements that make an ECS.
	 */
	class World {
		//VARIABLES
	private:
		clv::EventDispatcher ecsEventdispatcher;

		ComponentManager componentManager;

		std::vector<std::unique_ptr<System>> systems;

		static EntityID nextID;
		std::vector<EntityID> activeIDs;
		std::set<EntityID> pendingDestroyIDs;

		//FUNCTIONS
	public:
		World();

		World(const World& other) = delete;
		World(World&& other) noexcept = delete;

		World& operator=(const World& other) = delete;
		World& operator=(World&& other) noexcept = delete;

		~World();

		/**
		 * Update the ECS world
		 * Called once per frame. Goes through each System in the World
		 * and updates them
		 */
		void update(clv::utl::DeltaTime deltaTime);

		/**
		 * Creates an entity to be part of this World
		 * @return The newly created entity
		 */
		Entity createEntity();
		/**
		 * Clones an Entiy's Components, returning a new Entity with the same Component layout
		 * @param ID The Id of the entity to be cloned
		 * @return The newly cloned entity
		 */
		Entity cloneEntitiesComponents(EntityID ID);

		/**
		 * Checks if an Entity is valid
		 * An entity is valid if it is part of this World and has a valid EntityID
		 * @param ID The Entity to check
		 * @return Returns true if the entity is valid
		 */
		bool isEntityValid(EntityID ID);

		/**
		 * Returns an Entity wrapper for the given Id
		 * @param ID The entity to get
		 * @see Entity
		 * @return The Entity wrapper for the given ID
		 */
		Entity getEntity(EntityID ID);
		/**
		 * Gets all of the active Entities that are part of this World
		 * @return A vector of type Entity
		 */
		std::vector<Entity> getActiveEntities();

		/**
		 * Removes the Entity from this World
		 * Entities are removed the next time update is called
		 * @param ID The Entity to remove
		 */
		void destroyEntity(EntityID ID);
		/**
		 * Destroys all Entities in this World
		 * Entities are destroyed immediately
		 */
		void destroyAllEntites();

		/**
		 * Adds a Component to this world
		 * @tparam ComponentType The type of Component to add
		 * @param entityId The Entity to add the component to
		 * @param args Construction arguments to be forwarded to the component's constructor
		 * @return A ComponentPtr to the newly created component
		 * @see ComponentPtr
		 */
		template<typename ComponentType, typename... ConstructArgs>
		ComponentPtr<ComponentType> addComponent(EntityID entityId, ConstructArgs&&... args);
		/**
		 * Gets the Component for an Entity
		 * @tparam ComponentType The type of Component to get
		 * @param entityId The Entity to get the component from
		 * @return A ComponentPtr to the component if it exists on the Entity
		 * @see ComponentPtr
		 */
		template<typename ComponentType>
		ComponentPtr<ComponentType> getComponent(EntityID entityId);
		/**
		 * Checks to see if the component exists on an Entity
		 * @tparam ComponentType The type of the Component to check for
		 * @param entityId The entity to check
		 * @return Returns true if the Component exists on the Entity
		 */
		template<typename ComponentType>
		bool hasComponent(EntityID entityId);
		/**
		 * Removes a component from an Entity
		 * @tparam ComponentType The type of Component to remove
		 * @param entiyId The Entity to remove the Component from
		 */
		template<typename ComponentType>
		void removeComponent(EntityID entityId);

		/**
		 * Returns a vector of tuples of ComponentPtrs for each Entity matching the Components given
		 * Each element in the vector is a tuple. Each tuple contains the Components for  a single Entity.
		 * For example, if you call this with getComponentSets<TransformComponent, ModelComponent>()
		 * each tuple will represent a single Entity that has both a TransformComponent
		 * and a ModelComponent
		 * @tparam ComponentTypes A variadic template of the components needed
		 * @return A vector of tuples of Components for each entity that matches
		 * @see ComponentPtr
		 */
		template<typename... ComponentTypes>
		std::vector<std::tuple<ComponentPtr<ComponentTypes>...>> getComponentSets();

		/**
		 * Adds a System to the World
		 * @tparam SystemType The type of System to add
		 * @param args Construction arguments that'll be forwarded to the System
		 */
		template<typename SystemType, typename... ConstructArgs>
		void addSystem(ConstructArgs&&... args);

	private:
		enum class FoundState { NullptrFound, EndOfTuple };
		template<std::size_t index, typename... ComponentTypes>
		FoundState checkForNullptr(const std::tuple<ComponentPtr<ComponentTypes>...>& tuple);
	};
}

#include "World.inl"
