#pragma once

#include "Clove/ECS/Entity.hpp"

#include <Clove/Memory/PoolAllocator.hpp>
#include <memory>
#include <unordered_map>
#include <vector>

namespace garlic::clove {
    class EventDispatcher;
}

namespace garlic::clove {
    class ComponentContainerInterface {
        //FUNCTIONS
    public:
        virtual ~ComponentContainerInterface();

        virtual bool hasComponent(Entity entity) const = 0;

        virtual void cloneComponent(Entity from, Entity to) = 0;
        virtual void removeComponent(Entity entity)         = 0;
    };

    template<typename ComponentType>
    class ComponentContainer : public ComponentContainerInterface {
        //VARIABLES
    private:
        PoolAllocator<ComponentType, AllocatorStrategy::Dynamic> componentAllocator;

        std::unordered_map<Entity, size_t> entityToIndex;
        std::vector<ComponentType *> components;//Pointers into the componentAllocator for convienient lookup

        EventDispatcher *ecsEventDispatcher;

        //FUNCTIONS
    public:
        ComponentContainer() = delete;
        ComponentContainer(EventDispatcher *dispatcher);

        ComponentContainer(ComponentContainer const &other) = delete;
        ComponentContainer(ComponentContainer &&other) noexcept;

        ComponentContainer &operator=(ComponentContainer const &other) = delete;
        ComponentContainer &operator=(ComponentContainer &&other) noexcept;

        ~ComponentContainer();

        bool hasComponent(Entity entity) const final;

        void cloneComponent(Entity from, Entity to) final;
        void removeComponent(Entity entity) final;

        template<typename... ConstructArgs>
        ComponentType &addComponent(Entity entity, ConstructArgs &&... args);
        ComponentType &getComponent(Entity entity);
    };

    class ComponentManager {
        //TYPES
    private:
        using ComponentId = size_t;

        //VARIABLES
    private:
        std::unordered_map<ComponentId, std::unique_ptr<ComponentContainerInterface>> containers;

        EventDispatcher *ecsEventDispatcher;

        //FUNCTIONS
    public:
        ComponentManager() = delete;
        ComponentManager(EventDispatcher *dispatcher);

        ComponentManager(ComponentManager const &other)     = delete;
        ComponentManager(ComponentManager &&other) noexcept = delete;

        ComponentManager &operator=(ComponentManager const &other) = delete;
        ComponentManager &operator=(ComponentManager &&other) noexcept = delete;

        ~ComponentManager();

        template<typename ComponentType>
        ComponentContainer<ComponentType> &getComponentContainer();

        void cloneEntitiesComponents(Entity from, Entity to);

        void onEntityDestroyed(Entity entity);
    };
}

#include "ComponentManager.inl"