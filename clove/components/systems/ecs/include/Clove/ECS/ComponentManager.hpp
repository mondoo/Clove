#pragma once

#include "Clove/ECS/Entity.hpp"

#include <Clove/Memory/PoolAllocator.hpp>
#include <any>
#include <array>
#include <memory>
#include <type_traits>
#include <typeinfo>
#include <unordered_map>
#include <vector>

namespace garlic::clove {
    class EventDispatcher;

    struct ComponentId {
        using Type = size_t;

        template<typename ComponentType>
        static inline size_t const get() {
            static size_t const id{ typeid(ComponentType).hash_code() };
            return id;
        }
    };
}

namespace garlic::clove {
    class ComponentManager {
        //TYPES
    private:
        class ComponentContainerInterface {
        public:
            virtual bool canHold(std::vector<std::any> const &components) = 0;
            virtual bool canHold(std::type_info const &component)         = 0;

            virtual bool hasEntity(Entity entity)                                   = 0;
            virtual void addEntity(Entity entity, std::vector<std::any> components) = 0;
            virtual std::vector<std::any> removeEntity(Entity entity)               = 0;

            virtual void *getComponent(Entity entity, std::type_info const &info) = 0;
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
            std::unordered_map<Entity, size_t> entityToIndex{};            /**< Maps the Entity id to an index into entityComponents. */
            std::vector<std::tuple<ComponentTypes...>> entityComponents{}; /**< Contains a tuple of components for each entity. */

            //FUNCTIONS
        public:
            ComponentContainer() = default;

            ComponentContainer(ComponentContainer const &other)     = delete;
            ComponentContainer(ComponentContainer &&other) noexcept = default;

            ComponentContainer &operator=(ComponentContainer const &other) = delete;
            ComponentContainer &operator=(ComponentContainer &&other) noexcept = default;

            ~ComponentContainer() = default;

            bool canHold(std::vector<std::any> const &components) final {
                if(std::size(components) != sizeof...(ComponentTypes)) {
                    return false;
                }

                return areSame(components, std::make_index_sequence<sizeof...(ComponentTypes)>{});
            }

            bool canHold(std::type_info const &component) final {
                return ((typeid(ComponentTypes) == component) || ...);
            }

            bool hasEntity(Entity entity) final {
                return entityToIndex.find(entity) != entityToIndex.end();
            }

            void addEntity(Entity entity, std::vector<std::any> components) final {
                //TODO: Needed?
                //if(canHold(components)) {
                auto compTuple{ convert(components, std::make_index_sequence<sizeof...(ComponentTypes)>{}) };

                if(auto iter = entityToIndex.find(entity); iter != entityToIndex.end()) {
                    entityComponents[iter->second] = std::move(compTuple);
                } else {
                    entityComponents.emplace_back(std::move(compTuple));
                    entityToIndex[entity] = entityComponents.size() - 1;
                }
                //}
                //TODO: Error if not
            }

            std::vector<std::any> removeEntity(Entity entity) final {
                if(auto iter = entityToIndex.find(entity); iter != entityToIndex.end()) {
                    size_t const index{ iter->second };
                    size_t const lastIndex{ entityComponents.size() - 1 };

                    std::tuple<ComponentTypes...> removedComponents{ std::move(entityComponents[index]) };

                    if(index < lastIndex) {
                        entityComponents[index] = std::move(entityComponents.back());

                        //Update the index map so it knows about the moved component
                        for(auto [entityId, componentIndex] : entityToIndex) {
                            if(componentIndex == lastIndex) {
                                entityToIndex[entityId] = index;
                                break;
                            }
                        }
                    }
                    entityComponents.pop_back();
                    entityToIndex.erase(entity);

                    return { std::get<ComponentTypes>(removedComponents)... };
                }

                //TODO: Expected??
                return {};
            }

            // template<typename ComponentType>
            // bool hasComponent(Entity entity) {
            // }

            void *getComponent(Entity entity, std::type_info const &info) final {
                //TODO: Check entity is in here
                return getPointerToElement<0>(info, entityComponents[entityToIndex[entity]]);
            }

        private:
            template<size_t index>
            void *getPointerToElement(std::type_info const &elementType, std::tuple<ComponentTypes...> &searchTuple) {
                if constexpr(index < sizeof...(ComponentTypes)) {
                    auto &component{ std::get<index>(searchTuple) };
                    if(typeid(decltype(component)) == elementType) {
                        return &component;
                    } else {
                        return getPointerToElement<index + 1>(elementType, searchTuple);
                    }
                } else {
                    return nullptr;
                }
            }

            template<size_t... indices>
            bool areSame(std::vector<std::any> const &components, std::index_sequence<indices...>) {
                return ((components[indices].type() == typeid(ComponentTypes)) && ...);
            }

            template<size_t... indices>
            std::tuple<ComponentTypes...> convert(std::vector<std::any> const &components, std::index_sequence<indices...>) {
                return std::make_tuple(std::any_cast<std::tuple_element_t<indices, std::tuple<ComponentTypes...>>>(components[indices])...);
            }
        };

        //TODO: Temp name
        class TypeErasedContainer {
        public:
            std::unique_ptr<ComponentContainerInterface> container{};

        public:
            template<typename... ComponentTypes>
            static TypeErasedContainer create() {
                TypeErasedContainer container{};
                container.container = std::make_unique<ComponentContainer<ComponentTypes...>>();

                return container;
            }

            bool canHold(std::vector<std::any> const &components) {
                return container->canHold(components);
            }

            template<typename ComponentType>
            bool canHold() {
                return container->canHold(typeid(ComponentType));
            }

            bool hasEntity(Entity entity) {
                return container->hasEntity(entity);
            }

            void addEntity(Entity entity, std::vector<std::any> components) {
                container->addEntity(entity, std::move(components));
            }

            std::vector<std::any> removeEntity(Entity entity) {
                return container->removeEntity(entity);
            }

            template<typename ComponentType>
            ComponentType &getComponent(Entity entity) {
                //TODO: Check for nullptr
                return *reinterpret_cast<ComponentType *>(container->getComponent(entity, typeid(ComponentType)));
            }
        };

        //VARIABLES
    private:
        std::vector<TypeErasedContainer> containers;

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
        ComponentType &addComponent(Entity entity, ConstructArgs &&... args) {
            //TODO: Properly inforce or handle non copyable types?
            if constexpr(!std::is_copy_constructible_v<ComponentType>) {
                ComponentType *nullComp{ nullptr };
                return *nullComp;
            } else {
                for(auto &container : containers) {
                    if(container.hasEntity(entity)) {
                        if(container.canHold<ComponentType>()) {
                            ComponentType &comp{ container.getComponent<ComponentType>(entity) };
                            comp = std::move(ComponentType{ std::forward<ConstructArgs>(args)... });

                            return comp;
                        } else {
                            std::vector<std::any> components{ container.removeEntity(entity) };
                            ComponentType newComp{ std::forward<ConstructArgs>(args)... };
                            components.emplace_back(std::any{ std::move(newComp) });

                            //TODO: Skip current container
                            for(auto &otherContainer : containers) {
                                if(otherContainer.canHold(components)) {
                                    otherContainer.addEntity(entity, std::move(components));
                                    return container.getComponent<ComponentType>(entity);
                                }
                            }

                            //TODO
                            //Add expanded archetype container
                        }
                    }
                }

                auto newContainer{ TypeErasedContainer::create<ComponentType>() };
                newContainer.addEntity(entity, { ComponentType{ std::forward<ConstructArgs>(args)... } });
                containers.emplace_back(std::move(newContainer));

                return containers.back().getComponent<ComponentType>(entity);
            }
        }

        template<typename ComponentType>
        ComponentType &getComponent(Entity entity) {
            for(auto &container : containers) {
                if(container.hasEntity(entity)) {
                    return container.getComponent<ComponentType>(entity);
                }
            }

            //TODO: Handle
            ComponentType *nullComp{ nullptr };
            return *nullComp;
        }

        template<typename ComponentType>
        bool hasComponent(Entity entity) {
            return false;
        }

        template<typename ComponentType>
        void removeComponent(Entity entity) {
        }
    };
}

#include "ComponentManager.inl"