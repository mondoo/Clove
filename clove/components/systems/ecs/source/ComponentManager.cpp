#include "Clove/ECS/ComponentManager.hpp"

namespace garlic::clove {
    ComponentManager::ComponentContainerInterface::~ComponentContainerInterface() = default;

    ComponentManager::ComponentManager(EventDispatcher *dispatcher)
        : ecsEventDispatcher(dispatcher) {
    }

    ComponentManager::~ComponentManager() = default;

    void ComponentManager::cloneEntitiesComponents(Entity from, Entity to) {
        // for(auto &[key, container] : containers) {
        //     //container->cloneComponent(from, to);
        // }
    }

    void ComponentManager::onEntityDestroyed(Entity entity) {
        // for(auto &[key, container] : containers) {
        //     //container->removeComponent(entity);
        // }
    }
}
