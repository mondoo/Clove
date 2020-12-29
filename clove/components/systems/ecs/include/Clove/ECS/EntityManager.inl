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

    template<typename... ComponentTypes, typename... ExcludeTypes>
    void EntityManager::forEach(void (*updateFunction)(ComponentTypes...), Exclude<ExcludeTypes...>) {
        componentManager.generateView<std::decay_t<ComponentTypes>...>().forEach(updateFunction);
    }

    template<typename... ComponentTypes, typename... ExcludeTypes>
    void EntityManager::forEach(void (*updateFunction)(Entity, ComponentTypes...), Exclude<ExcludeTypes...>) {
        for(Entity entity : activeEntities) {
            if(Exclude<ExcludeTypes...>::size > 0) {
                if((hasComponent<std::remove_const_t<std::remove_reference_t<ExcludeTypes>>>(entity) || ...)) {
                    continue;
                }
            }

            if((hasComponent<std::remove_const_t<std::remove_reference_t<ComponentTypes>>>(entity) && ...)) {
                (*updateFunction)(entity, getComponent<std::remove_const_t<std::remove_reference_t<ComponentTypes>>>(entity)...);
            }
        }
    }

    template<typename SystemType, typename... ComponentTypes, typename... ExcludeTypes>
    void EntityManager::forEach(void (SystemType::*updateFunction)(ComponentTypes...), SystemType *system, Exclude<ExcludeTypes...>) {
        for(Entity entity : activeEntities) {
            if(Exclude<ExcludeTypes...>::size > 0) {
                if((hasComponent<std::remove_const_t<std::remove_reference_t<ExcludeTypes>>>(entity) || ...)) {
                    continue;
                }
            }

            if((hasComponent<std::remove_const_t<std::remove_reference_t<ComponentTypes>>>(entity) && ...)) {
                (system->*updateFunction)(getComponent<std::remove_const_t<std::remove_reference_t<ComponentTypes>>>(entity)...);
            }
        }
    }

    template<typename SystemType, typename... ComponentTypes, typename... ExcludeTypes>
    void EntityManager::forEach(void (SystemType::*updateFunction)(Entity, ComponentTypes...), SystemType *system, Exclude<ExcludeTypes...>) {
        for(Entity entity : activeEntities) {
            if(Exclude<ExcludeTypes...>::size > 0) {
                if((hasComponent<std::remove_const_t<std::remove_reference_t<ExcludeTypes>>>(entity) || ...)) {
                    continue;
                }
            }

            if((hasComponent<std::remove_const_t<std::remove_reference_t<ComponentTypes>>>(entity) && ...)) {
                (system->*updateFunction)(entity, getComponent<std::remove_const_t<std::remove_reference_t<ComponentTypes>>>(entity)...);
            }
        }
    }

    template<typename SystemType, typename... ComponentTypes, typename... ExcludeTypes>
    void EntityManager::forEach(void (SystemType::*updateFunction)(ComponentTypes...) const, SystemType *system, Exclude<ExcludeTypes...>) {
        for(Entity entity : activeEntities) {
            if(Exclude<ExcludeTypes...>::size > 0) {
                if((hasComponent<std::remove_const_t<std::remove_reference_t<ExcludeTypes>>>(entity) || ...)) {
                    continue;
                }
            }

            if((hasComponent<std::remove_const_t<std::remove_reference_t<ComponentTypes>>>(entity) && ...)) {
                (system->*updateFunction)(getComponent<std::remove_const_t<std::remove_reference_t<ComponentTypes>>>(entity)...);
            }
        }
    }

    template<typename SystemType, typename... ComponentTypes, typename... ExcludeTypes>
    void EntityManager::forEach(void (SystemType::*updateFunction)(Entity, ComponentTypes...) const, SystemType *system, Exclude<ExcludeTypes...>) {
        for(Entity entity : activeEntities) {
            if(Exclude<ExcludeTypes...>::size > 0) {
                if((hasComponent<std::remove_const_t<std::remove_reference_t<ExcludeTypes>>>(entity) || ...)) {
                    continue;
                }
            }

            if((hasComponent<std::remove_const_t<std::remove_reference_t<ComponentTypes>>>(entity) && ...)) {
                (system->*updateFunction)(entity, getComponent<std::remove_const_t<std::remove_reference_t<ComponentTypes>>>(entity)...);
            }
        }
    }

    template<typename SystemType, typename... ComponentTypes, typename... ExcludeTypes>
    void EntityManager::forEach(void (SystemType::*updateFunction)(ComponentTypes...) const, SystemType system, Exclude<ExcludeTypes...>) {
        for(Entity entity : activeEntities) {
            if(Exclude<ExcludeTypes...>::size > 0) {
                if((hasComponent<std::remove_const_t<std::remove_reference_t<ExcludeTypes>>>(entity) || ...)) {
                    continue;
                }
            }

            if((hasComponent<std::remove_const_t<std::remove_reference_t<ComponentTypes>>>(entity) && ...)) {
                (system.*updateFunction)(getComponent<std::remove_const_t<std::remove_reference_t<ComponentTypes>>>(entity)...);
            }
        }
    }

    template<typename SystemType, typename... ComponentTypes, typename... ExcludeTypes>
    void EntityManager::forEach(void (SystemType::*updateFunction)(Entity, ComponentTypes...) const, SystemType system, Exclude<ExcludeTypes...>) {
        for(Entity entity : activeEntities) {
            if(Exclude<ExcludeTypes...>::size > 0) {
                if((hasComponent<std::remove_const_t<std::remove_reference_t<ExcludeTypes>>>(entity) || ...)) {
                    continue;
                }
            }

            if((hasComponent<std::remove_const_t<std::remove_reference_t<ComponentTypes>>>(entity) && ...)) {
                (system.*updateFunction)(entity, getComponent<std::remove_const_t<std::remove_reference_t<ComponentTypes>>>(entity)...);
            }
        }
    }

    template<typename CallableType, typename... ExcludeTypes>
    void EntityManager::forEach(CallableType callable, Exclude<ExcludeTypes...>) {
        forEach(&CallableType::operator(), std::move(callable), Exclude<ExcludeTypes...>{});
    }
}