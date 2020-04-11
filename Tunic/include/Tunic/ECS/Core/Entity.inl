#include "Tunic/ECS/Core/World.hpp"

namespace tnc::ecs{
	template<typename ComponentType, typename ...ConstructorArgs>
	ComponentPtr<ComponentType> Entity::addComponent(ConstructorArgs&&... args) {
		if(isValid()){
			return manager->addComponent<ComponentType>(getID(), args...);
		} else{
			return {};
		}
	}
	
	template<typename ComponentType>
	ComponentPtr<ComponentType> Entity::getComponent() const {
		if(isValid()){
			return manager->getComponent<ComponentType>(getID());
		} else{
			return {};
		}
	}

	template<typename ComponentType>
	void Entity::removeComponent(){
		if(isValid()){
			manager->removeComponent<ComponentType>(getID());
		}
	}
}
