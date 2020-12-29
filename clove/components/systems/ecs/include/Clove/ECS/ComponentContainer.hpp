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
        //TYPES
    public:
        template<typename T>
        using ContainerType = std::vector<T>;

        using IndexType = typename ContainerType<ComponentType>::size_type;
        //VARIABLES
    private:
        static inline IndexType constexpr nullIndex{ ~0u };

        ContainerType<IndexType> entityToIndex{}; /**< Sparse vector of indices. Used to index into either packed array. Entity is an index into this array */

        ContainerType<Entity> entities{};          /**< Packed vector of entities. Used when iterating over just the entities this container is aware of. */
        ContainerType<ComponentType> components{}; /**< Packed vector of components. Storage for components and used when iterating over just the components this container owns. */

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