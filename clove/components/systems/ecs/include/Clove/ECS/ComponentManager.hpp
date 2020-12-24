#pragma once

#include "Clove/ECS/Entity.hpp"

#include <Clove/Memory/PoolAllocator.hpp>
#include <array>
#include <memory>
#include <unordered_map>
#include <vector>
#include <typeinfo>

namespace garlic::clove {
    class EventDispatcher;

    struct ComponentId {
        using type = size_t;

        template<typename ComponentType>
        static inline type const get() {
            static type const id{ typeid(ComponentType).hash_code() };
            return id;
        }
    };

}

namespace garlic::clove {
    class ComponentManager {
        //TYPES
    private:
        /**
         * @brief Interface class to allow component containers to be stored in an array.
         */
        class ComponentContainerInterface {
            //FUNCTIONS
        public:
            virtual ~ComponentContainerInterface();

            virtual bool hasEntity(Entity entity) = 0;

            //virtual void addEntity(Entity entity)    = 0;
            virtual void removeEntity(Entity entity) = 0;

            virtual void *getComponent(Entity entity, ComponentId::type id) = 0;

            template<typename ComponentType>
            ComponentType *getComponent(Entity entity) {
                return reinterpret_cast<ComponentType *>(getComponent(entity, ComponentId::get<ComponentType>()));
            }

            virtual void cloneEntity(Entity from, Entity to) = 0;
        };

        /**
         * @brief Contains all componenties for an entity of a specific component composition.
         * @details Whenever a component is added or removed from an entitiy it is moved to a 
         * different container. This ensures cache friendly access when iterating over components
         * of a certain type.
         * @tparam ComponentTypes The components that make up an entity's type.
         */
        template<typename... ComponentTypes>
        class ComponentContainer : public ComponentContainerInterface {
            //VARIABLES
        private:
            /**
             * @brief Maps the Entity id to an index into entityComponents
             */
            std::unordered_map<Entity, size_t> entityToIndex;
            std::vector<std::tuple<ComponentTypes...>> entityComponents;

            /**
             * @brief Contains the Ids of the components this container holds.
             */
            //std::array<ComponentId_t, sizeof...(ComponentTypes)> const componentIds{ ComponentId<ComponentTypes>::value()... };

            //FUNCTIONS
        public:
            ComponentContainer();

            ComponentContainer(ComponentContainer const &other) = delete;
            ComponentContainer(ComponentContainer &&other) noexcept;

            ComponentContainer &operator=(ComponentContainer const &other) = delete;
            ComponentContainer &operator=(ComponentContainer &&other) noexcept;

            ~ComponentContainer();

            bool hasEntity(Entity entity) final;

            void addEntity(Entity entity, ComponentTypes &&... components) /* final */;
            void removeEntity(Entity entity) final;

            void *getComponent(Entity entity, ComponentId::type id) final;

            void cloneEntity(Entity from, Entity to) final;
        };

        //VARIABLES
    private:
        std::vector<std::unique_ptr<ComponentContainerInterface>> containers;

        EventDispatcher *ecsEventDispatcher{ nullptr };

        //FUNCTIONS
    public:
        ComponentManager() = delete;
        ComponentManager(EventDispatcher *dispatcher);

        ComponentManager(ComponentManager const &other)     = delete;
        ComponentManager(ComponentManager &&other) noexcept = delete;

        ComponentManager &operator=(ComponentManager const &other) = delete;
        ComponentManager &operator=(ComponentManager &&other) noexcept = delete;

        ~ComponentManager();

        template<typename ComponentType, typename... ConstructArgs>
        ComponentType &addComponent(Entity entity, ConstructArgs &&... args);
        template<typename ComponentType>
        ComponentType &getComponent(Entity entity);

        template<typename ComponentType>
        bool hasComponent(Entity entity);

        template<typename ComponentType>
        void removeComponent(Entity entity);

        void cloneEntitiesComponents(Entity from, Entity to);

        void onEntityDestroyed(Entity entity);

        //TODO:
        //template<typename ComponentTypes...>
        //iterType createIterator()
    };
}

#include "ComponentManager.inl"