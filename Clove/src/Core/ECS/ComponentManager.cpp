#include "ComponentManager.hpp"

namespace clv::ecs{
	ComponentManager::ComponentContainerInterface::ComponentContainerInterface() = default;

	ComponentManager::ComponentContainerInterface::ComponentContainerInterface(const ComponentContainerInterface& other) = default;

	ComponentManager::ComponentContainerInterface::ComponentContainerInterface(ComponentContainerInterface&& other) noexcept = default;

	ComponentManager::ComponentContainerInterface& ComponentManager::ComponentContainerInterface::operator=(const ComponentContainerInterface& other) = default;

	ComponentManager::ComponentContainerInterface& ComponentManager::ComponentContainerInterface::operator=(ComponentContainerInterface&& other) noexcept = default;

	ComponentManager::ComponentContainerInterface::~ComponentContainerInterface() = default;

	ComponentManager::ComponentManager() = default;

	ComponentManager::~ComponentManager() = default;

	void ComponentManager::onEntityDestroyed(EntityID entityID){
		for(auto& [key, container] : containers){
			container->removeComponent(entityID);
		}
	}
	
	void ComponentManager::onContainerAddedComponent(ComponentInterface* component){
		componentAddedDelegate.broadcast(component);
	}

	void ComponentManager::onContainerRemovedComponent(ComponentInterface* component){
		componentRemovedDelegate.broadcast(component);
	}
}

