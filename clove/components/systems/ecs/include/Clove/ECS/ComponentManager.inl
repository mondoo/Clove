#include "Clove/ECS/ECSEvents.hpp"

#include <Clove/Definitions.hpp>
#include <Clove/Event/EventDispatcher.hpp>
#include <Clove/Log/Log.hpp>

namespace garlic::clove {
    template<typename ComponentType>
    ComponentManager::ComponentContainer<ComponentType>::ComponentContainer(EventDispatcher *dispatcher)
        : ecsEventDispatcher{ dispatcher } {
    }

    template<typename ComponentType>
    ComponentManager::ComponentContainer<ComponentType>::ComponentContainer(ComponentContainer &&other) noexcept = default;

    template<typename ComponentType>
    ComponentManager::ComponentContainer<ComponentType> &ComponentManager::ComponentContainer<ComponentType>::operator=(ComponentContainer &&other) noexcept = default;

    template<typename ComponentType>
    ComponentManager::ComponentContainer<ComponentType>::~ComponentContainer() = default;

    template<typename ComponentType>
    bool ComponentManager::ComponentContainer<ComponentType>::hasComponent(Entity entity) const {
        auto iter{ entityToIndex.find(entity) };
        return iter != entityToIndex.end();
    }

    template<typename ComponentType>
    void ComponentManager::ComponentContainer<ComponentType>::cloneComponent(Entity from, Entity to) {
        if constexpr(std::is_copy_constructible_v<ComponentType>) {
            if(auto iter = entityToIndex.find(from); iter != entityToIndex.end()) {
                ComponentType &component{ components[iter->second] };
                addComponent(to, component);
            }
        } else {
            CLOVE_LOG(LOG_CATEGORY_CLOVE, LogLevel::Error, "Component that is not copyable was attempted to be copied. Entity will be incomplete");
        }
    }

    template<typename ComponentType>
    void ComponentManager::ComponentContainer<ComponentType>::removeComponent(Entity entity) {
        if(auto iter = entityToIndex.find(entity); iter != entityToIndex.end()) {
            size_t const index{ iter->second };
            size_t const lastIndex{ components.size() - 1 };

            ComponentType removedComp{ std::move(components[index]) };

            if(index < lastIndex) {
                components[index] = std::move(components.back());

                //Update the index map so it knows about the moved component
                for(auto [entityId, componentIndex] : entityToIndex) {
                    if(componentIndex == lastIndex) {
                        entityToIndex[entityId] = index;
                        break;
                    }
                }
            }
            components.pop_back();
            entityToIndex.erase(entity);

            ecsEventDispatcher->broadCastEvent(ComponentRemovedEvent<ComponentType>{ entity, removedComp });
        }
    }

    template<typename ComponentType>
    template<typename... ConstructArgs>
    ComponentType &ComponentManager::ComponentContainer<ComponentType>::addComponent(Entity entity, ConstructArgs &&... args) {
        ComponentType *addedComp{ nullptr };

        if(auto iter = entityToIndex.find(entity); iter != entityToIndex.end()) {
            CLOVE_LOG_DEBUG(LOG_CATEGORY_CLOVE, LogLevel::Warning, "{0} was called on an Entity that alread has that component. Old component will be replaced with the new one", CLOVE_FUNCTION_NAME_PRETTY);
            components[iter->second] = ComponentType{ std::forward<ConstructArgs>(args)... };
            addedComp                = &components[iter->second];
        } else {
            components.emplace_back(ComponentType{ std::forward<ConstructArgs>(args)... });
            entityToIndex[entity] = components.size() - 1;
            addedComp             = &components.back();
        }

        ecsEventDispatcher->broadCastEvent(ComponentAddedEvent<ComponentType>{ entity, *addedComp });

        return *addedComp;
    }

    template<typename ComponentType>
    ComponentType &ComponentManager::ComponentContainer<ComponentType>::getComponent(Entity entity) {
        if(auto iter = entityToIndex.find(entity); iter != entityToIndex.end()) {
            return components[iter->second];
        } else {
            //TODO: return Excepted type?
            CLOVE_ASSERT(false, "Entity does not have component of specified type!");
            ComponentType *nullComp{ nullptr };
            return *nullComp;
        }
    }

    template<typename ComponentType>
    ComponentManager::ComponentContainer<ComponentType> &ComponentManager::getComponentContainer() {
        ComponentId const componentId{ typeid(ComponentType).hash_code() };
        if(auto iter = containers.find(componentId); iter != containers.end()) {
            return static_cast<ComponentContainer<ComponentType> &>(*iter->second.get());
        } else {
            auto container{ std::make_unique<ComponentContainer<ComponentType>>(ecsEventDispatcher) };

            containers[componentId] = std::move(container);
            return static_cast<ComponentContainer<ComponentType> &>(*containers[componentId].get());
        }
    }
}