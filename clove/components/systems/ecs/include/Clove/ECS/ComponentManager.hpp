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
        };

        template<typename ComponentType>
        class Container : public ContainerInterface {
            //VARIABLES
        private:
            std::unordered_map<Entity, size_t> entityToIndex{};
            std::vector<ComponentType> components{};

            //FUNCTIONS
        public:
            Container() = default;

            Container(Container const &other)     = delete;
            Container(Container &&other) noexcept = default;

            Container &operator=(Container const &other) = delete;
            Container &operator=(Container &&other) noexcept = default;

            ~Container() = default;

            bool hasComponent(Entity entity) const {
                return entityToIndex.find(entity) != entityToIndex.end();
            }

            void cloneComponent(Entity from, Entity to) {
            }
            void removeComponent(Entity entity) {
            }

            template<typename... ConstructArgs>
            ComponentType &addComponent(Entity entity, ConstructArgs &&... args) {
                if(auto iter = entityToIndex.find(entity); iter != entityToIndex.end()) {
                    components[iter->second] = ComponentType{ std::forward<ConstructArgs>(args)... };
                    
                    CLOVE_LOG(LOG_CATEGORY_CLOVE, LogLevel::Debug, "{0} was called for Entity {1} that alread has that component. Old component is replaced with a new one", CLOVE_FUNCTION_NAME_PRETTY, entity);
                    
                    return components[iter->second];
                } else {
                    components.emplace_back(ComponentType{ std::forward<ConstructArgs>(args)... });
                    entityToIndex[entity] = components.size() - 1;
                    
                    CLOVE_LOG(LOG_CATEGORY_CLOVE, LogLevel::Trace, "{0} added new entry for Entity {1}. Index in array is {2}", CLOVE_FUNCTION_NAME_PRETTY, entity, entityToIndex[entity]);
                    
                    return components[entityToIndex[entity]];
                }
            }
            ComponentType &getComponent(Entity entity) {
                CLOVE_ASSERT(hasComponent(entity), "{0}: Entity does not have component", CLOVE_FUNCTION_NAME_PRETTY);
                return components[entityToIndex[entity]];
            }
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
        ComponentType &addComponent(Entity entity, ConstructArgs &&... args) {
            return getContainer<ComponentType>().addComponent(entity, std::forward<ConstructArgs>(args)...);
        }

        template<typename ComponentType>
        ComponentType &getComponent(Entity entity) {
            return getContainer<ComponentType>().getComponent(entity);
        }

        template<typename ComponentType>
        bool hasComponent(Entity entity) {
            return getContainer<ComponentType>().hasComponent(entity);
        }

        template<typename ComponentType>
        void removeComponent(Entity entity) {
        }

    private:
        template<typename ComponentType>
        Container<ComponentType> &getContainer() {
            std::type_index const componentIndex{ typeid(ComponentType) };

            if(auto iter{ containers.find(componentIndex) }; iter != containers.end()) {
                return static_cast<Container<ComponentType> &>(*iter->second.get());
            } else {
                containers[componentIndex] = std::make_unique<Container<ComponentType>>();
                return static_cast<Container<ComponentType> &>(*containers[componentIndex].get());
            }
        }
    };
}

#include "ComponentManager.inl"