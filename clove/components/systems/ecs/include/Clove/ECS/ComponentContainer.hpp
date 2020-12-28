#pragma once

#include "Clove/ECS/Entity.hpp"

#include <unordered_map>
#include <vector>

namespace garlic::clove {
    class ComponentContainerInterface {
        //FUNCTIONS
    public:
        virtual ~ComponentContainerInterface() = default;

        virtual bool hasComponent(Entity entity) const      = 0;
        virtual void cloneComponent(Entity from, Entity to) = 0;

        virtual void removeComponent(Entity entity) = 0;
    };

    template<typename ComponentType>
    class ComponentContainer : public ComponentContainerInterface {
        //VARIABLES
    private:
        std::unordered_map<Entity, size_t> entityToIndex{};
        std::vector<ComponentType> components{};

        //FUNCTIONS
    public:
        ComponentContainer();

        ComponentContainer(ComponentContainer const &other) = delete;
        ComponentContainer(ComponentContainer &&other) noexcept;

        ComponentContainer &operator=(ComponentContainer const &other) = delete;
        ComponentContainer &operator=(ComponentContainer &&other) noexcept;

        ~ComponentContainer();

        template<typename... ConstructArgs>
        ComponentType &addComponent(Entity entity, ConstructArgs &&... args);
        ComponentType &getComponent(Entity entity);

        bool hasComponent(Entity entity) const final;
        void cloneComponent(Entity from, Entity to) final;
        void removeComponent(Entity entity) final;
    };
}

#include "ComponentContainer.inl"