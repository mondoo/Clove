#include <Clove/Timer.hpp>

namespace garlic::clove {
    EventDispatcher &World::getDispatcher() {
        return ecsEventDispatcher;
    }

    template<typename ComponentType, typename... ConstructArgs>
    ComponentPtr<ComponentType> World::addComponent(Entity entity, ConstructArgs &&... args) {
        return componentManager.getComponentContainer<ComponentType>().addComponent(entity, std::forward<ConstructArgs>(args)...);
    }

    template<typename ComponentType>
    ComponentPtr<ComponentType> World::getComponent(Entity entity) {
        return componentManager.getComponentContainer<ComponentType>().getComponent(entity);
    }

    template<typename ComponentType>
    bool World::hasComponent(Entity entity) {
        return componentManager.getComponentContainer<ComponentType>().hasComponent(entity);
    }

    template<typename ComponentType>
    void World::removeComponent(Entity entity) {
        componentManager.getComponentContainer<ComponentType>().removeComponent(entity);
    }

    template<typename... ComponentTypes>
    void World::forEach(void (*updateFunction)(ComponentTypes...)) {
        for(Entity entity : activeEntities) {
            if((hasComponent<std::remove_const_t<std::remove_reference_t<ComponentTypes>>>(entity) && ...)) {
                (*updateFunction)(*getComponent<std::remove_const_t<std::remove_reference_t<ComponentTypes>>>(entity)...);
            }
        }
    }

    template<typename SystemType, typename... ComponentTypes>
    void World::forEach(void (SystemType::*updateFunction)(ComponentTypes...), SystemType *system) {
        for(Entity entity : activeEntities) {
            if((hasComponent<std::remove_const_t<std::remove_reference_t<ComponentTypes>>>(entity) && ...)) {
                (system->*updateFunction)(*getComponent<std::remove_const_t<std::remove_reference_t<ComponentTypes>>>(entity)...);
            }
        }
    }

    template<typename SystemType, typename... ComponentTypes>
    void World::forEach(void (SystemType::*updateFunction)(ComponentTypes...) const, SystemType *system) {
        for(Entity entity : activeEntities) {
            if((hasComponent<std::remove_const_t<std::remove_reference_t<ComponentTypes>>>(entity) && ...)) {
                (system->*updateFunction)(*getComponent<std::remove_const_t<std::remove_reference_t<ComponentTypes>>>(entity)...);
            }
        }
    }

    template<typename SystemType, typename... ComponentTypes>
    void World::forEach(void (SystemType::*updateFunction)(ComponentTypes...) const, SystemType system) {
        for(Entity entity : activeEntities) {
            if((hasComponent<std::remove_const_t<std::remove_reference_t<ComponentTypes>>>(entity) && ...)) {
                (system.*updateFunction)(*getComponent<std::remove_const_t<std::remove_reference_t<ComponentTypes>>>(entity)...);
            }
        }
    }

    template<typename CallableType>
    void World::forEach(CallableType callable) {
        forEach(&CallableType::operator(), std::move(callable));
    }
}