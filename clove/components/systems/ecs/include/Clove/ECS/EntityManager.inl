#include <Clove/Timer.hpp>

namespace garlic::clove {
    EventDispatcher &EntityManager::getDispatcher() {
        return ecsEventDispatcher;
    }

    template<typename ComponentType, typename... ConstructArgs>
    ComponentType &EntityManager::addComponent(Entity entity, ConstructArgs &&... args) {
        CLOVE_ASSERT(isValid(entity), "{0}: Invalid entity provided", CLOVE_FUNCTION_NAME_PRETTY);
        return componentManager.addComponent<ComponentType>(entity, std::forward<ConstructArgs>(args)...);
    }

    template<typename ComponentType>
    ComponentType &EntityManager::getComponent(Entity entity) {
        CLOVE_ASSERT(isValid(entity), "{0}: Invalid entity provided", CLOVE_FUNCTION_NAME_PRETTY);
        return componentManager.getComponent<ComponentType>(entity);
    }

    template<typename ComponentType>
    bool EntityManager::hasComponent(Entity entity) {
        if(isValid(entity)) {
            return componentManager.hasComponent<ComponentType>(entity);
        } else {
            return false;
        }
    }

    template<typename ComponentType>
    void EntityManager::removeComponent(Entity entity) {
        if(isValid(entity)) {
            componentManager.removeComponent<ComponentType>(entity);
        }
    }
}