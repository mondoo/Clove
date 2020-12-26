#include "Clove/ECS/ECSEvents.hpp"

#include <Clove/Definitions.hpp>
#include <Clove/Event/EventDispatcher.hpp>
#include <Clove/Log/Log.hpp>

namespace garlic::clove {
    // template<typename... ComponentTypes>
    // ComponentManager::ComponentContainer<ComponentTypes...>::ComponentContainer() = default;

    // template<typename... ComponentTypes>
    // ComponentManager::ComponentContainer<ComponentTypes...>::ComponentContainer(ComponentContainer &&other) noexcept = default;

    // template<typename... ComponentTypes>
    // ComponentManager::ComponentContainer<ComponentTypes...> &ComponentManager::ComponentContainer<ComponentTypes...>::operator=(ComponentContainer &&other) noexcept = default;

    // template<typename... ComponentTypes>
    // ComponentManager::ComponentContainer<ComponentTypes...>::~ComponentContainer() = default;

    // template<typename... ComponentTypes>
    // bool ComponentManager::ComponentContainer<ComponentTypes...>::hasEntity(Entity entity) {
    //     return entityToIndex.find(entity) != entityToIndex.end();
    // }

    // template<typename... ComponentTypes>
    // void ComponentManager::ComponentContainer<ComponentTypes...>::addEntity(Entity entity, ComponentTypes &&... components) {
    //     addEntity(entity, std::make_tuple<ComponentTypes...>(std::forward<ComponentTypes>(components)...));
    // }

    // template<typename... ComponentTypes>
    // void ComponentManager::ComponentContainer<ComponentTypes...>::addEntity(Entity entity, std::tuple<ComponentTypes...> &&components) {
    //     if(auto iter = entityToIndex.find(entity); iter != entityToIndex.end()) {
    //         entityComponents[iter->second] = std::move(components);
    //     } else {
    //         entityComponents.emplace_back(std::move(components));
    //         entityToIndex[entity] = entityComponents.size() - 1;
    //     }
    // }

    // template<typename... ComponentTypes>
    // std::tuple<ComponentTypes...> ComponentManager::ComponentContainer<ComponentTypes...>::removeEntity(Entity entity) {
    //     if(auto iter = entityToIndex.find(entity); iter != entityToIndex.end()) {
    //         size_t const index{ iter->second };
    //         size_t const lastIndex{ entityComponents.size() - 1 };

    //         std::tuple<ComponentTypes...> removedComponents{ std::move(entityComponents[index]) };

    //         if(index < lastIndex) {
    //             entityComponents[index] = std::move(entityComponents.back());

    //             //Update the index map so it knows about the moved component
    //             for(auto [entityId, componentIndex] : entityToIndex) {
    //                 if(componentIndex == lastIndex) {
    //                     entityToIndex[entityId] = index;
    //                     break;
    //                 }
    //             }
    //         }
    //         entityComponents.pop_back();
    //         entityToIndex.erase(entity);

    //         return removedComponents;
    //     }

    //     //TODO: Expected??
    //     return {};
    // }

    // template<typename ComponentType, typename... ConstructArgs>
    // ComponentType &ComponentManager::addComponent(Entity entity, ConstructArgs &&... args) {
    //     ComponentContainerInterface *entityContainer{ nullptr };
    //     for(auto &container : containers) {
    //         if(container->hasEntity(entity)) {
    //             entityContainer = container.get();
    //             break;
    //         }
    //     }

    //     if(entityContainer != nullptr) {
    //         if(entityContainer->canStoreComponent<ComponentType>()) {
    //             return entityContainer->getComponent<ComponentType>(entity);
    //         } /*  else {
    //             auto containerType{ entityContainer->getContainerType() };
    //             containerType.push_back(ComponentId::get<ComponentType>());
    //         } */
    //     }

    //     std::unique_ptr<ComponentContainerInterface> container{ std::make_unique<ComponentContainer<ComponentType>>() };

    //     container->addEntity(entity, ComponentType{ std::forward<ConstructArgs>(args)... });
    //     return container->getComponent<ComponentType>(entity);
    // }

    // template<typename ComponentType>
    // ComponentType &ComponentManager::getComponent(Entity entity) {
    //     // if(auto iter = entityToIndex.find(entity); iter != entityToIndex.end()) {
    //     //     return components[iter->second];
    //     // } else {
    //     //     //TODO: return Excepted type?
    //     //     CLOVE_ASSERT(false, "Entity does not have component of specified type!");
    //     //     ComponentType *nullComp{ nullptr };
    //     //     return *nullComp;
    //     // }

    //     ComponentType *nullComp{ nullptr };
    //     return *nullComp;
    // }

    // template<typename ComponentType>
    // bool ComponentManager::hasComponent(Entity entity) {
    //     for(auto &container : containers) {
    //         if(container->hasEntity(entity)) {
    //             return container->hasComponent<ComponentType>(entity);
    //         }
    //     }

    //     return false;
    // }

    // template<typename ComponentType>
    // void ComponentManager::removeComponent(Entity entity) {
    //     //ecsEventDispatcher->broadCastEvent(ComponentRemovedEvent<ComponentType>{ entity, removedComp });
    // }
}