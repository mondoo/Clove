#include "Tunic/ECS/World.hpp"

namespace tnc::ecs{
	template<typename ComponentType, typename ...ConstructorArgs>
	ComponentPtr<ComponentType> Entity::addComponent(ConstructorArgs&&... args) {
		if(isValid()){
			return world->addComponent<ComponentType>(getID(), args...);
		} else{
			return {};
		}
	}
	
	template<typename ComponentType>
	ComponentPtr<ComponentType> Entity::getComponent() const {
		if(isValid()){
			return world->getComponent<ComponentType>(getID());
		} else{
			return {};
		}
	}

	template<typename ComponentType>
	void Entity::removeComponent(){
		if(isValid()){
			world->removeComponent<ComponentType>(getID());
		}
	}
}
