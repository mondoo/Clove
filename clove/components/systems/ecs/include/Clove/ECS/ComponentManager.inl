#include "Clove/ECS/ECSEvents.hpp"

#include <Clove/Definitions.hpp>
#include <Clove/Event/EventDispatcher.hpp>
#include <Clove/Log/Log.hpp>

namespace garlic::clove {
    namespace {
        template<size_t index = 0, typename... ComponentTypes>
        void *getPointerToElement(std::tuple<ComponentTypes...> &tuple, ComponentId::type findId) {
            if constexpr(index < sizeof...(ComponentTypes)) {
                auto &component{ std::get<index>(tuple) };
                if(ComponentId::get<decltype(component)>() == findId) {
                    return &component;
                } else {
                    return getPointerToElement<index + 1, ComponentTypes...>(tuple, findId);
                }
            } else {
                return nullptr;
            }
        }
    }

    template<typename... ComponentTypes>
    ComponentManager::ComponentContainer<ComponentTypes...>::ComponentContainer() = default;

    template<typename... ComponentTypes>
    ComponentManager::ComponentContainer<ComponentTypes...>::ComponentContainer(ComponentContainer &&other) noexcept = default;

    template<typename... ComponentTypes>
    ComponentManager::ComponentContainer<ComponentTypes...> &ComponentManager::ComponentContainer<ComponentTypes...>::operator=(ComponentContainer &&other) noexcept = default;

    template<typename... ComponentTypes>
    ComponentManager::ComponentContainer<ComponentTypes...>::~ComponentContainer() = default;

    template<typename... ComponentTypes>
    bool ComponentManager::ComponentContainer<ComponentTypes...>::hasEntity(Entity entity) {
        return entityToIndex.find(entity) != entityToIndex.end();
    }

    template<typename... ComponentTypes>
    void ComponentManager::ComponentContainer<ComponentTypes...>::addEntity(Entity entity, ComponentTypes &&... components) {
        if(auto iter = entityToIndex.find(entity); iter == entityToIndex.end()) {
            entityComponents.emplace_back(std::tuple<ComponentTypes...>{ std::forward<ComponentTypes>(components)... });
            entityToIndex[entity] = entityComponents.size() - 1;
        }
    }

    template<typename... ComponentTypes>
    void ComponentManager::ComponentContainer<ComponentTypes...>::removeEntity(Entity entity) {
        //     if(auto iter = entityToIndex.find(entity); iter != entityToIndex.end()) {
        //         size_t const index{ iter->second };
        //         size_t const lastIndex{ components.size() - 1 };

        //         ComponentType removedComp{ std::move(components[index]) };

        //         if(index < lastIndex) {
        //             components[index] = std::move(components.back());

        //             //Update the index map so it knows about the moved component
        //             for(auto [entityId, componentIndex] : entityToIndex) {
        //                 if(componentIndex == lastIndex) {
        //                     entityToIndex[entityId] = index;
        //                     break;
        //                 }
        //             }
        //         }
        //         components.pop_back();
        //         entityToIndex.erase(entity);

        //         ecsEventDispatcher->broadCastEvent(ComponentRemovedEvent<ComponentType>{ entity, removedComp });
        //     }
    }

    template<typename... ComponentTypes>
    void *ComponentManager::ComponentContainer<ComponentTypes...>::getComponent(Entity entity, ComponentId::type id) {
        if(auto iter = entityToIndex.find(entity); iter != entityToIndex.end()) {
            std::tuple<ComponentTypes...> &components{ entityComponents[entityToIndex[entity]] };
            return getPointerToElement(components, id);
        } else {
            return nullptr;
        }
    }

    template<typename... ComponentTypes>
    void ComponentManager::ComponentContainer<ComponentTypes...>::cloneEntity(Entity from, Entity to) {
    }

    template<typename ComponentType, typename... ConstructArgs>
    ComponentType &ComponentManager::addComponent(Entity entity, ConstructArgs &&... args) {
        //TEMP: Just adding them straight in for now
        auto container{ std::make_unique<ComponentContainer<ComponentType>>() };
        container->addEntity(entity, ComponentType{ std::forward<ConstructArgs>(args)... });

        //auto *comp{ container->getComponent<ComponentType>(entity) };
        auto *comp{ reinterpret_cast<ComponentType *>(container->getComponent(entity, ComponentId::get<ComponentType>())) };

        containers.emplace_back(std::move(container));

        //TODO: Broadcast event

        return *comp;
    }

    template<typename ComponentType>
    ComponentType &ComponentManager::getComponent(Entity entity) {
        // if(auto iter = entityToIndex.find(entity); iter != entityToIndex.end()) {
        //     return components[iter->second];
        // } else {
        //     //TODO: return Excepted type?
        //     CLOVE_ASSERT(false, "Entity does not have component of specified type!");
        //     ComponentType *nullComp{ nullptr };
        //     return *nullComp;
        // }

        ComponentType *nullComp{ nullptr };
        return *nullComp;
    }

    template<typename ComponentType>
    bool ComponentManager::hasComponent(Entity entity) {
        for(auto& container : containers){
            if(container->hasEntity(entity)){
                return container->getComponent<ComponentType>(entity) != nullptr;
            }
        }

        return false;
    }

    template<typename ComponentType>
    void ComponentManager::removeComponent(Entity entity) {
    }
}