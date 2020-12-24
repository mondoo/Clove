#include "Clove/ECS/ECSEvents.hpp"

#include <Clove/Definitions.hpp>
#include <Clove/Event/EventDispatcher.hpp>
#include <Clove/Log/Log.hpp>

namespace garlic::clove {
    template<typename ComponentType>
    ComponentContainer<ComponentType>::ComponentContainer(EventDispatcher *dispatcher)
        : ecsEventDispatcher{ dispatcher }
        , componentAllocator{ 100 } {
    }

    template<typename ComponentType>
    ComponentContainer<ComponentType>::ComponentContainer(ComponentContainer &&other) noexcept = default;

    template<typename ComponentType>
    ComponentContainer<ComponentType> &ComponentContainer<ComponentType>::operator=(ComponentContainer &&other) noexcept = default;

    template<typename ComponentType>
    ComponentContainer<ComponentType>::~ComponentContainer() = default;

    template<typename ComponentType>
    bool ComponentContainer<ComponentType>::hasComponent(Entity entity) const {
        if(auto iter = entityToIndex.find(entity); iter != entityToIndex.end()) {
            return components[iter->second] != nullptr;
        } else {
            return false;
        }
    }

    template<typename ComponentType>
    void ComponentContainer<ComponentType>::cloneComponent(Entity from, Entity to) {
        if constexpr(std::is_copy_constructible_v<ComponentType>) {
            if(auto iter = entityToIndex.find(from); iter != entityToIndex.end()) {
                ComponentType *componentPtr = components[iter->second];
                addComponent(to, *componentPtr);
            }
        } else {
            CLOVE_LOG(LOG_CATEGORY_CLOVE, LogLevel::Error, "Component that is not copyable was attempted to be copied. Entity will be incomplete");
        }
    }

    template<typename ComponentType>
    void ComponentContainer<ComponentType>::removeComponent(Entity entity) {
        if(auto iter = entityToIndex.find(entity); iter != entityToIndex.end()) {
            size_t const index{ iter->second };
            size_t const lastIndex{ components.size() - 1 };

            ComponentType *removedComp = components[index];

            if(index < lastIndex) {
                components[index] = components.back();
            }
            components.pop_back();
            entityToIndex.erase(entity);

            //Update the index map so it knows about the moved component
            if(index < lastIndex) {
                Entity movedCompEntity{ components[index]->entity };
                entityToIndex[movedCompEntity] = index;
            }

            ecsEventDispatcher->broadCastEvent(ComponentRemovedEvent<ComponentType>{ removedComp });

            componentAllocator.free(removedComp);
        }
    }

    template<typename ComponentType>
    template<typename... ConstructArgs>
    ComponentPtr<ComponentType> ComponentContainer<ComponentType>::addComponent(Entity entity, ConstructArgs &&... args) {
        ComponentType *comp = componentAllocator.alloc(std::forward<ConstructArgs>(args)...);
        if(comp == nullptr) {
            CLOVE_LOG(LOG_CATEGORY_CLOVE, LogLevel::Error, "{0}: Could not create component", CLOVE_FUNCTION_NAME_PRETTY);
            return { comp };
        }

        comp->entity = entity;

        if(auto iter = entityToIndex.find(entity); iter != entityToIndex.end()) {
            CLOVE_LOG(LOG_CATEGORY_CLOVE, LogLevel::Debug, "{0} was called on an Entity that alread has that component. Old component will be replaced with the new one", CLOVE_FUNCTION_NAME_PRETTY);
            components[iter->second] = comp;
        } else {
            components.push_back(comp);
            entityToIndex[entity] = components.size() - 1;
        }

        ComponentAddedEvent<ComponentType> event{ comp };
        ecsEventDispatcher->broadCastEvent<ComponentAddedEvent<ComponentType>>(event);

        return { comp };
    }

    template<typename ComponentType>
    ComponentPtr<ComponentType> ComponentContainer<ComponentType>::getComponent(Entity entity) {
        if(auto iter = entityToIndex.find(entity); iter != entityToIndex.end()) {
            return { components[iter->second] };
        } else {
            return {};
        }
    }

    template<typename ComponentType>
    ComponentContainer<ComponentType> &ComponentManager::getComponentContainer() {
        ComponentID const componentID{ ComponentType::id() };
        if(auto iter = containers.find(componentID); iter != containers.end()) {
            return static_cast<ComponentContainer<ComponentType> &>(*iter->second.get());
        } else {
            auto container{ std::make_unique<ComponentContainer<ComponentType>>(ecsEventDispatcher) };

            containers[componentID] = std::move(container);
            return static_cast<ComponentContainer<ComponentType> &>(*containers[componentID].get());
        }
    }
}