#pragma once

#include "Clove/ECS/Entity.hpp"

#include <Clove/Memory/PoolAllocator.hpp>
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
        /**
         * @brief Contains all componenties for an entity of a specific component composition.
         * @details Whenever a component is added or removed from an entitiy it is moved to a 
         * different container. This ensures cache friendly access when iterating over components
         * of a certain type.
         * @tparam ComponentTypes The components that make up an entity's type.
         */
        //template<typename... ComponentTypes>
        class ComponentContainer /* : public ComponentContainerInterface */ {
            //TYPES
        private:
            struct ComponentMetaData {
                ComponentId::Type id;
                size_t size;
                size_t alignment;
            };

            //VARIABLES
        private:
            using TupleType = std::tuple<int>;

            //TODO: std::byte
            std::vector<char> buffer{};                         /**< Raw buffer of component types. */
            std::unordered_map<Entity, size_t> entityToIndex{}; /**< Maps the Entity id to an index into entityComponents. */
            std::vector<ComponentMetaData> const data{};        /**< MetaData about a single element. */

            //FUNCTIONS
        public:
            ComponentContainer(std::vector<ComponentMetaData> data, size_t stride)
                : data{ std::move(data) } {
            }

            ComponentContainer(ComponentContainer const &other)     = delete;
            ComponentContainer(ComponentContainer &&other) noexcept = default;

            ComponentContainer &operator=(ComponentContainer const &other) = delete;
            ComponentContainer &operator=(ComponentContainer &&other) noexcept = default;

            ~ComponentContainer() = default;

            template<typename ComponentType>
            static ComponentContainer create() {
                ComponentMetaData data{};
                data.id        = ComponentId::get<ComponentType>();
                data.size      = sizeof(ComponentType);
                data.alignment = alignof(ComponentType);

                return { { std::move(data) } };
            }

            template<typename ComponentType>
            static ComponentContainer appendType(ComponentContainer const &from) {
                ComponentMetaData data{};
                data.id        = ComponentId::get<ComponentType>();
                data.size      = sizeof(ComponentType);
                data.alignment = alignof(ComponentType);

                auto elementData{ from.data };

                elementData.push_back(data);

                return { { std::move(elementData) } };
            }

            bool hasEntity(Entity entity) /* final */ {
                return entityToIndex.find(entity) != entityToIndex.end();
            }

            void addEntity(Entity entity, void *components) {
                size_t const totalSize{ std::size(buffer) };
                buffer.resize(totalSize + getElementSize());

                char *back{ &buffer.back() };
                back -= getElementSize();

                memcpy(back, components, getElementSize());
            }

            void *removeEntity(Entity entity) {
                size_t const index{ entityToIndex[entity] };
                size_t const lastIndex{ (std::size(data) / getElementSize()) - 1 };

                char *element{ std::data(buffer) + index * getElementSize() };

                char *data = reinterpret_cast<char *>(malloc(getElementSize()));
                memcpy(data, element, getElementSize());

                if(index < lastIndex) {
                    char *back{ &buffer.back() };
                    back -= getElementSize();
                    memcpy(element, back, getElementSize());

                    //Update the index map so it knows about the moved component
                    for(auto [entityId, componentIndex] : entityToIndex) {
                        if(componentIndex == lastIndex) {
                            entityToIndex[entityId] = index;
                            break;
                        }
                    }
                }

                size_t const totalSize{ std::size(buffer) };
                buffer.resize(totalSize - getElementSize());

                return data;
            }

            template<typename ComponentType>
            bool hasComponent(Entity entity) {
                return canStoreComponent<ComponentType>() && hasEntity(entity);
            }

            template<typename ComponentType>
            bool canStoreComponent() {
                for(auto id : data) {
                    if(id.id == ComponentId::get<ComponentType>()) {
                        return true;
                    }
                }
                return false;
            }

            template<typename ComponentType>
            ComponentType &getComponent(Entity entity) {
                CLOVE_ASSERT(hasEntity(entity), "");
                size_t const index{ entityToIndex[entity] };
                char *const element{ std::data(buffer) + index * getElementSize() };

                char *component{ element };
                for(size_t i{ 0 }; i < std::size(data); ++i) {
                    if(data[i].id != ComponentId::get<ComponentType>()) {
                        component += data[i].size;
                    }
                }

                return reinterpret_cast<ComponentType *>(component);
            }

        private:
            size_t getElementSize() {
                size_t size{ 0 };
                for(auto id : data) {
                    size += id.size;
                }
                return size;
            }
        };

        //VARIABLES
    private:
        std::vector<ComponentContainer> containers;

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
            ComponentType *nullComp{ nullptr };
            return *nullComp;
        }

        template<typename ComponentType>
        ComponentType &getComponent(Entity entity) {
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

        //void cloneEntitiesComponents(Entity from, Entity to);

        //void onEntityDestroyed(Entity entity);

        //TODO:
        //template<typename ComponentTypes...>
        //iterType createIterator()
    };
}

#include "ComponentManager.inl"