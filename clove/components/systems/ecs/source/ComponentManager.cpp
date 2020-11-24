#include "Clove/ECS/ComponentManager.hpp"

namespace garlic::clove {
    ComponentContainerInterface::~ComponentContainerInterface() = default;

    ComponentManager::ComponentManager(EventDispatcher *dispatcher)
        : ecsEventDispatcher(dispatcher) {
    }

    ComponentManager::~ComponentManager() = default;

    void ComponentManager::cloneEntitiesComponents(EntityID fromID, EntityID toID) {
        for(auto &[key, container] : containers) {
            container->cloneComponent(fromID, toID);
        }
    }

    void ComponentManager::onEntityDestroyed(EntityID entityID) {
        for(auto &[key, container] : containers) {
            container->removeComponent(entityID);
        }
    }
}
