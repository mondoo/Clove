#pragma once

#include "Clove/ECS/Entity.hpp"

#include <Clove/Definitions.hpp>
#include <Clove/Log/Log.hpp>
#include <memory>
#include <type_traits>
#include <typeindex>
#include <typeinfo>
#include <unordered_map>
#include <vector>

namespace garlic::clove {
    class EventDispatcher;
}

namespace garlic::clove {
    class ComponentManager {
        //TYPES
    private:
        class ContainerInterface {
            //FUNCTIONS
        public:
            virtual ~ContainerInterface() = default;

            virtual bool hasComponent(Entity entity) const      = 0;
            virtual void cloneComponent(Entity from, Entity to) = 0;

            virtual void removeComponent(Entity entity) = 0;
        };

        template<typename ComponentType>
        class Container : public ContainerInterface {
            //VARIABLES
        private:
            std::unordered_map<Entity, size_t> entityToIndex{};
            std::vector<ComponentType> components{};

            //FUNCTIONS
        public:
            Container();

            Container(Container const &other) = delete;
            Container(Container &&other) noexcept;

            Container &operator=(Container const &other) = delete;
            Container &operator=(Container &&other) noexcept;

            ~Container();

            template<typename... ConstructArgs>
            ComponentType &addComponent(Entity entity, ConstructArgs &&... args);
            ComponentType &getComponent(Entity entity);

            bool hasComponent(Entity entity) const final;
            void cloneComponent(Entity from, Entity to) final;
            void removeComponent(Entity entity) final;
        };

        //VARIABLES
    private:
        std::unordered_map<std::type_index, std::unique_ptr<ContainerInterface>> containers{};

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
        void cloneComponents(Entity from, Entity to);
        template<typename ComponentType>
        void removeComponent(Entity entity);

        void removeEntity(Entity entity);

    private:
        template<typename ComponentType>
        Container<ComponentType> &getContainer();
    };
}

#include "ComponentManager.inl"