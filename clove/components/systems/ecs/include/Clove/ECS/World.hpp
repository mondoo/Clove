#pragma once

#include "Clove/ECS/ComponentManager.hpp"
#include "Clove/ECS/ComponentSet.hpp"
#include "Clove/ECS/ECSTypes.hpp"
#include "Clove/ECS/System.hpp"

#include <Clove/Event/EventDispatcher.hpp>
#include <set>
#include <vector>

namespace garlic::clove {
    class Entity;
    class System;
}

namespace garlic::clove {
    /**
	 * @brief The ECS World that contains the Entities Systems and Components.
	 * @details A World acts as a container that stores all of the elements that make an ECS.
	 */
    class World {
        //VARIABLES
    private:
        EventDispatcher ecsEventDispatcher;

        ComponentManager componentManager;

        std::vector<std::unique_ptr<System>> systems;

        static EntityID nextID;
        std::vector<EntityID> activeIDs;
        std::set<EntityID> pendingDestroyIDs;

        //FUNCTIONS
    public:
        World();

        World(World const &other)     = delete;
        World(World &&other) noexcept = delete;

        World &operator=(World const &other) = delete;
        World &operator=(World &&other) noexcept = delete;

        ~World();

        /**
		 * @brief Update the ECS world.
		 * @details Called once per frame. Goes through each System in the World
		 * and updates them.
		 */
        void update(DeltaTime deltaTime);

        /**
		 * @brief Creates an entity to be part of this World.
		 * @return The newly created entity.
		 */
        Entity createEntity();
        /**
		 * @brief Clones an Entiy's Components, returning a new Entity with the same Component layout.
		 * @param ID The Id of the entity to be cloned.
		 * @return The newly cloned entity.
		 */
        Entity cloneEntitiesComponents(EntityID ID);

        /**
		 * @brief Checks if an Entity is valid.
		 * @details An entity is valid if it is part of this World and has a valid EntityID.
		 * @param ID The Entity to check.
		 * @return Returns true if the entity is valid.
		 */
        bool isEntityValid(EntityID ID);

        /**
		 * @brief Returns an Entity wrapper for the given Id.
		 * @param ID The entity to get.
		 * @return The Entity wrapper for the given ID.
		 * @see	Entity.
		 */
        Entity getEntity(EntityID ID);
        /**
		 * @brief Gets all of the active Entities that are part of this World.
		 * @return A vector of type Entity.
		 */
        std::vector<Entity> getActiveEntities();

        /**
		 * @brief Removes the Entity from this World.
		 * @details Entities are removed the next time update is called.
		 * @param ID The Entity to remove.
		 */
        void destroyEntity(EntityID ID);
        /**
		 * @brief Destroys all Entities in this World.
		 * @details Entities are destroyed immediately.
		 */
        void destroyAllEntites();

        /**
		 * @brief Adds a Component to this world.
		 * @tparam ComponentType The type of Component to add.
		 * @param entityId The Entity to add the component to.
		 * @param args Construction arguments to be forwarded to the component's constructor.
		 * @return A ComponentPtr to the newly created component.
		 * @see	ComponentPtr.
		 */
        template<typename ComponentType, typename... ConstructArgs>
        ComponentPtr<ComponentType> addComponent(EntityID entityId, ConstructArgs &&... args);
        /**
		 * @brief Gets the Component for an Entity.
		 * @tparam ComponentType The type of Component to get.
		 * @param entityId The Entity to get the component from.
		 * @return A ComponentPtr to the component if it exists on the Entity.
		 * @see ComponentPtr.
		 */
        template<typename ComponentType>
        ComponentPtr<ComponentType> getComponent(EntityID entityId);
        /**
		 * @brief Checks to see if the component exists on an Entity.
		 * @tparam ComponentType The type of the Component to check for.
		 * @param entityId The entity to check.
		 * @return Returns true if the Component exists on the Entity.
		 */
        template<typename ComponentType>
        bool hasComponent(EntityID entityId);
        /**
		 * @brief Removes a component from an Entity.
		 * @tparam ComponentType The type of Component to remove.
		 * @param entiyId The Entity to remove the Component from.
		 */
        template<typename ComponentType>
        void removeComponent(EntityID entityId);

        /**
		 * @brief Finds all entitys that have the same components as ComponentTypes and returns them as a ComponentSet.
		 * @details	A ComponentSet is an array of tuples where each tuple are the components for a given entity.
		 * @tparam ComponentTypes A variadic template of the components needed.
		 * @return A vector of tuples of Components for each entity that matches.
		 * @see	ComponentPtr.
		 * @see ComponentSet.
		 */
        template<typename... ComponentTypes>
        ComponentSet<ComponentTypes...> getComponentSets();

        /**
		 * @brief Adds a System to the World.
		 * @note The lifetime of the System is tied to the World
		 * @tparam SystemType The type of System to add.
		 * @param args Construction arguments that'll be forwarded to the System.
		 * @return Returns a pointer to the System.
		 */
        template<typename SystemType, typename... ConstructArgs>
        SystemType *addSystem(ConstructArgs &&... args);

    private:
        enum class FoundState { NullptrFound,
                                EndOfTuple };
        template<std::size_t index, typename... ComponentTypes>
        FoundState checkForNullptr(std::tuple<ComponentPtr<ComponentTypes>...> const &tuple);
    };
}

#include "World.inl"
