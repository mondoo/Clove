#pragma once

#include "Clove/ECS/Component.hpp"
#include "Clove/ECS/ECSTypes.hpp"

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

        virtual bool hasComponent(EntityID entityId) const = 0;

        virtual void cloneComponent(EntityID fromId, EntityID toId) = 0;
        virtual void removeComponent(EntityID entityId)             = 0;
    };

    template<typename ComponentType>
    class ComponentContainer : public ComponentContainerInterface {
        //VARIABLES
    private:
        PoolAllocator<ComponentType, AllocatorStrategy::Dynamic> componentAllocator;

        std::unordered_map<EntityID, size_t> entityIDToIndex;
        std::vector<ComponentType *> components;

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

        bool hasComponent(EntityID entityId) const final;

        void cloneComponent(EntityID fromId, EntityID toId) final;
        void removeComponent(EntityID entityId) final;

        template<typename... ConstructArgs>
        ComponentPtr<ComponentType> addComponent(EntityID entityId, ConstructArgs &&... args);
        ComponentPtr<ComponentType> getComponent(EntityID entityId);
    };

    class ComponentManager {
        //VARIABLES
    private:
        std::unordered_map<ComponentID, std::unique_ptr<ComponentContainerInterface>> containers;

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

        void cloneEntitiesComponents(EntityID fromID, EntityID toID);

        void onEntityDestroyed(EntityID entityID);
    };
}

#include "ComponentManager.inl"