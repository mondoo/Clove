#include <Clove/Timer.hpp>

namespace garlic::clove {
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
}