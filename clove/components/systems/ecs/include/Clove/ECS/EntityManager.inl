#include <Clove/Timer.hpp>

namespace garlic::clove {
    EventDispatcher &EntityManager::getDispatcher() {
        return ecsEventDispatcher;
    }

    template<typename ComponentType, typename... ConstructArgs>
    ComponentType &EntityManager::addComponent(Entity entity, ConstructArgs &&... args) {
        return componentManager.getComponentContainer<ComponentType>().addComponent(entity, std::forward<ConstructArgs>(args)...);
    }

    template<typename ComponentType>
    ComponentType &EntityManager::getComponent(Entity entity) {
        return componentManager.getComponentContainer<ComponentType>().getComponent(entity);
    }

    template<typename ComponentType>
    bool EntityManager::hasComponent(Entity entity) {
        return componentManager.getComponentContainer<ComponentType>().hasComponent(entity);
    }

    template<typename ComponentType>
    void EntityManager::removeComponent(Entity entity) {
        componentManager.getComponentContainer<ComponentType>().removeComponent(entity);
    }

    template<typename... ComponentTypes>
    void EntityManager::forEach(void (*updateFunction)(ComponentTypes...)) {
        for(Entity entity : activeEntities) {
            if((hasComponent<std::remove_const_t<std::remove_reference_t<ComponentTypes>>>(entity) && ...)) {
                (*updateFunction)(getComponent<std::remove_const_t<std::remove_reference_t<ComponentTypes>>>(entity)...);
            }
        }
    }

    template<typename... ComponentTypes>
    void EntityManager::forEach(void (*updateFunction)(Entity, ComponentTypes...)) {
        for(Entity entity : activeEntities) {
            if((hasComponent<std::remove_const_t<std::remove_reference_t<ComponentTypes>>>(entity) && ...)) {
                (*updateFunction)(entity, getComponent<std::remove_const_t<std::remove_reference_t<ComponentTypes>>>(entity)...);
            }
        }
    }

    template<typename SystemType, typename... ComponentTypes>
    void EntityManager::forEach(void (SystemType::*updateFunction)(ComponentTypes...), SystemType *system) {
        for(Entity entity : activeEntities) {
            if((hasComponent<std::remove_const_t<std::remove_reference_t<ComponentTypes>>>(entity) && ...)) {
                (system->*updateFunction)(getComponent<std::remove_const_t<std::remove_reference_t<ComponentTypes>>>(entity)...);
            }
        }
    }

    template<typename SystemType, typename... ComponentTypes>
    void EntityManager::forEach(void (SystemType::*updateFunction)(Entity, ComponentTypes...), SystemType *system) {
        for(Entity entity : activeEntities) {
            if((hasComponent<std::remove_const_t<std::remove_reference_t<ComponentTypes>>>(entity) && ...)) {
                (system->*updateFunction)(entity, getComponent<std::remove_const_t<std::remove_reference_t<ComponentTypes>>>(entity)...);
            }
        }
    }

    template<typename SystemType, typename... ComponentTypes>
    void EntityManager::forEach(void (SystemType::*updateFunction)(ComponentTypes...) const, SystemType *system) {
        for(Entity entity : activeEntities) {
            if((hasComponent<std::remove_const_t<std::remove_reference_t<ComponentTypes>>>(entity) && ...)) {
                (system->*updateFunction)(getComponent<std::remove_const_t<std::remove_reference_t<ComponentTypes>>>(entity)...);
            }
        }
    }

    template<typename SystemType, typename... ComponentTypes>
    void EntityManager::forEach(void (SystemType::*updateFunction)(Entity, ComponentTypes...) const, SystemType *system) {
        for(Entity entity : activeEntities) {
            if((hasComponent<std::remove_const_t<std::remove_reference_t<ComponentTypes>>>(entity) && ...)) {
                (system->*updateFunction)(entity, getComponent<std::remove_const_t<std::remove_reference_t<ComponentTypes>>>(entity)...);
            }
        }
    }

    template<typename SystemType, typename... ComponentTypes>
    void EntityManager::forEach(void (SystemType::*updateFunction)(ComponentTypes...) const, SystemType system) {
        for(Entity entity : activeEntities) {
            if((hasComponent<std::remove_const_t<std::remove_reference_t<ComponentTypes>>>(entity) && ...)) {
                (system.*updateFunction)(getComponent<std::remove_const_t<std::remove_reference_t<ComponentTypes>>>(entity)...);
            }
        }
    }

    template<typename SystemType, typename... ComponentTypes>
    void EntityManager::forEach(void (SystemType::*updateFunction)(Entity, ComponentTypes...) const, SystemType system) {
        for(Entity entity : activeEntities) {
            if((hasComponent<std::remove_const_t<std::remove_reference_t<ComponentTypes>>>(entity) && ...)) {
                (system.*updateFunction)(entity, getComponent<std::remove_const_t<std::remove_reference_t<ComponentTypes>>>(entity)...);
            }
        }
    }

    template<typename CallableType>
    void EntityManager::forEach(CallableType callable) {
        forEach(&CallableType::operator(), std::move(callable));
    }
}