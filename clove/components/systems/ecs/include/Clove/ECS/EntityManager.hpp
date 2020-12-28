#pragma once

#include "Clove/ECS/ComponentManager.hpp"
#include "Clove/ECS/Entity.hpp"

#include <Clove/Event/EventDispatcher.hpp>
#include <set>
#include <vector>

namespace garlic::clove {
    template<typename... Types>
    struct Exclude {
        static size_t constexpr size{ sizeof...(Types) };
    };
}

namespace garlic::clove {
    /**
	 * @brief The EntityManager that contains the Entities and their Components.
	 */
    class EntityManager {
        //VARIABLES
    private:
        EventDispatcher ecsEventDispatcher;

        ComponentManager componentManager;

        static Entity nextEntity;
        std::vector<Entity> activeEntities;
        std::set<Entity> pendingDestroyEntities;

        //FUNCTIONS
    public:
        EntityManager();

        EntityManager(EntityManager const &other)     = delete;
        EntityManager(EntityManager &&other) noexcept = delete;

        EntityManager &operator=(EntityManager const &other) = delete;
        EntityManager &operator=(EntityManager &&other) noexcept = delete;

        ~EntityManager();

        inline EventDispatcher &getDispatcher();

        /**
		 * @brief Creates an entity to be part of this EntityManager.
		 * @return The newly created entity.
		 */
        Entity create();
        /**
		 * @brief Clones an Entiy's Components, returning a new Entity with the same Component layout.
		 * @param ID The Id of the entity to be cloned.
		 * @return The newly cloned entity.
		 */
        Entity clone(Entity entity);

        /**
		 * @brief Removes the Entity from this EntityManager.
		 * @details Entities are removed the next time update is called.
		 * @param ID The Entity to remove.
		 */
        void destroy(Entity entity);
        /**
		 * @brief Destroys all Entities in this EntityManager.
		 * @details Entities are destroyed immediately.
		 */
        void destroyAll();

        /**
		 * @brief Checks if an Entity is valid.
		 * @details An entity is valid if it is part of this EntityManager and has a valid EntityID.
		 * @param ID The Entity to check.
		 * @return Returns true if the entity is valid.
		 */
        bool isValid(Entity entity);

        /**
		 * @brief Adds a Component to this entityManager.
		 * @tparam ComponentType The type of Component to add.
		 * @param entityId The Entity to add the component to.
		 * @param args Construction arguments to be forwarded to the component's constructor.
		 * @return A ComponentPtr to the newly created component.
		 * @see	ComponentPtr.
		 */
        template<typename ComponentType, typename... ConstructArgs>
        ComponentType &addComponent(Entity entity, ConstructArgs &&... args);
        /**
		 * @brief Gets the Component for an Entity.
		 * @tparam ComponentType The type of Component to get.
		 * @param entityId The Entity to get the component from.
		 * @return A ComponentPtr to the component if it exists on the Entity.
		 * @see ComponentPtr.
		 */
        template<typename ComponentType>
        ComponentType &getComponent(Entity entity);
        /**
		 * @brief Checks to see if the component exists on an Entity.
		 * @tparam ComponentType The type of the Component to check for.
		 * @param entityId The entity to check.
		 * @return Returns true if the Component exists on the Entity.
		 */
        template<typename ComponentType>
        bool hasComponent(Entity entity);
        /**
		 * @brief Removes a component from an Entity.
		 * @tparam ComponentType The type of Component to remove.
		 * @param entiyId The Entity to remove the Component from.
		 */
        template<typename ComponentType>
        void removeComponent(Entity entity);

        /**
         * @brief Calls the function for every Entity in the entityManager.
         * @tparam ComponentTypes The components to iterate over.
         * @tparam ExcludeTypes The components to exclude from this iteration.
         */
        template<typename... ComponentTypes, typename... ExcludeTypes>
        void forEach(void (*updateFunction)(ComponentTypes...), Exclude<ExcludeTypes...> = {});
        /*! @copydoc forEach */
        template<typename... ComponentTypes, typename... ExcludeTypes>
        void forEach(void (*updateFunction)(Entity, ComponentTypes...), Exclude<ExcludeTypes...> = {});

        /*! @copydoc forEach */
        template<typename SystemType, typename... ComponentTypes, typename... ExcludeTypes>
        void forEach(void (SystemType::*updateFunction)(ComponentTypes...), SystemType *system, Exclude<ExcludeTypes...> = {});
        /*! @copydoc forEach */
        template<typename SystemType, typename... ComponentTypes, typename... ExcludeTypes>
        void forEach(void (SystemType::*updateFunction)(Entity, ComponentTypes...), SystemType *system, Exclude<ExcludeTypes...> = {});

        /*! @copydoc forEach */
        template<typename SystemType, typename... ComponentTypes, typename... ExcludeTypes>
        void forEach(void (SystemType::*updateFunction)(ComponentTypes...) const, SystemType *system, Exclude<ExcludeTypes...> = {});
        /*! @copydoc forEach */
        template<typename SystemType, typename... ComponentTypes, typename... ExcludeTypes>
        void forEach(void (SystemType::*updateFunction)(Entity, ComponentTypes...) const, SystemType *system, Exclude<ExcludeTypes...> = {});

        /*! @copydoc forEach */
        template<typename SystemType, typename... ComponentTypes, typename... ExcludeTypes>
        void forEach(void (SystemType::*updateFunction)(ComponentTypes...) const, SystemType system, Exclude<ExcludeTypes...> = {});
        /*! @copydoc forEach */
        template<typename SystemType, typename... ComponentTypes, typename... ExcludeTypes>
        void forEach(void (SystemType::*updateFunction)(Entity, ComponentTypes...) const, SystemType system, Exclude<ExcludeTypes...> = {});

        /**
         * @brief Takes a callable type (such as a lambda) and calls it for every Entity in the entityManager.
         * @tparam ExcludeTypes The components to exclude from this iteration.
         */
        template<typename CallableType, typename... ExcludeTypes>
        void forEach(CallableType callable, Exclude<ExcludeTypes...> = {});
    };
}

#include "EntityManager.inl"
