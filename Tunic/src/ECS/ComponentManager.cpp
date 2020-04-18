#include "Tunic/ECS/ComponentManager.hpp"

namespace tnc::ecs {
	ComponentContainerInterface::~ComponentContainerInterface() = default;

	ComponentManager::ComponentManager() = default;

	ComponentManager::~ComponentManager() = default;

	void ComponentManager::cloneEntitiesComponents(EntityID fromID, EntityID toID) {
		for(auto& [key, container] : containers) {
			container->cloneComponent(fromID, toID);
		}
	}

	void ComponentManager::onEntityDestroyed(EntityID entityID) {
		for(auto& [key, container] : containers) {
			container->removeComponent(entityID);
		}
	}

	void ComponentManager::onContainerAddedComponent(ComponentInterface* component) {
		componentAddedDelegate.broadcast(component);
	}

	void ComponentManager::onContainerRemovedComponent(ComponentInterface* component) {
		componentRemovedDelegate.broadcast(component);
	}
}
