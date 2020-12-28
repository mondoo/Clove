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

            virtual bool hasComponent(Entity entity) const = 0;

            virtual void cloneComponent(Entity from, Entity to) = 0;
            virtual void removeComponent(Entity entity)         = 0;
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

            bool hasComponent(Entity entity) const final {
                return false;
            }

            void cloneComponent(Entity from, Entity to) final {
            }
            void removeComponent(Entity entity) final {
            }

            template<typename... ConstructArgs>
            ComponentType &addComponent(Entity entity, ConstructArgs &&... args) {
                ComponentType *addedComp{ nullptr };

                if(auto iter = entityToIndex.find(entity); iter != entityToIndex.end()) {
                    CLOVE_LOG(LOG_CATEGORY_CLOVE, LogLevel::Debug, "{0} was called on an Entity that alread has that component. Old component will be replaced with the new one", CLOVE_FUNCTION_NAME_PRETTY);
                    components[iter->second] = ComponentType{ std::forward<ConstructArgs>(args)... };
                    addedComp                = &components[iter->second];
                } else {
                    components.emplace_back(ComponentType{ std::forward<ConstructArgs>(args)... });
                    entityToIndex[entity] = components.size() - 1;
                    addedComp             = &components.back();
                }

                return *addedComp;
            }
            ComponentType &getComponent(Entity entity) {
            }
        };

        //VARIABLES
    private:
        std::unordered_map<std::type_index, std::unique_ptr<ContainerInterface>> containers;

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