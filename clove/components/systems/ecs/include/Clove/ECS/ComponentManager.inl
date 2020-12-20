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

            ComponentType *const removedComp{ components[index] };

            if(index < lastIndex) {
                components[index] = components.back();

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

            ecsEventDispatcher->broadCastEvent(ComponentRemovedEvent<ComponentType>{ entity, *removedComp });

            componentAllocator.free(removedComp);
        }
    }

    template<typename ComponentType>
    template<typename... ConstructArgs>
    ComponentType &ComponentContainer<ComponentType>::addComponent(Entity entity, ConstructArgs &&... args) {
        ComponentType *comp{ componentAllocator.alloc(std::forward<ConstructArgs>(args)...) };
        if(comp == nullptr) {
            CLOVE_LOG(LOG_CATEGORY_CLOVE, LogLevel::Error, "{0}: Could not create component", CLOVE_FUNCTION_NAME_PRETTY);
            return *comp;
        }

        if(auto iter = entityToIndex.find(entity); iter != entityToIndex.end()) {
            CLOVE_LOG_DEBUG(LOG_CATEGORY_CLOVE, LogLevel::Warning, "{0} was called on an Entity that alread has that component. Old component will be replaced with the new one", CLOVE_FUNCTION_NAME_PRETTY);
            components[iter->second] = comp;
        } else {
            components.push_back(comp);
            entityToIndex[entity] = components.size() - 1;
        }

        ecsEventDispatcher->broadCastEvent(ComponentAddedEvent<ComponentType>{ entity, *comp });

        return *comp;
    }

    template<typename ComponentType>
    ComponentType &ComponentContainer<ComponentType>::getComponent(Entity entity) {
        if(auto iter = entityToIndex.find(entity); iter != entityToIndex.end()) {
            return *components[iter->second];
        } else {
            //TODO: return Excepted type?
            CLOVE_ASSERT(false, "Entity does not have component of specified type!");
            ComponentType *nullComp{ nullptr };
            return *nullComp;
        }
    }

    template<typename ComponentType>
    ComponentContainer<ComponentType> &ComponentManager::getComponentContainer() {
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