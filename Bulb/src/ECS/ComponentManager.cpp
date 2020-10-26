#include "Bulb/ECS/ComponentManager.hpp"

namespace blb::ecs {
    ComponentContainerInterface::~ComponentContainerInterface() = default;

    ComponentManager::ComponentManager(garlic::EventDispatcher *dispatcher)
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
