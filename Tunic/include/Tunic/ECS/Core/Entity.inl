#include "Tunic/ECS/Core/World.hpp"

namespace tnc::ecs{
	template<typename ComponentType, typename ...ConstructorArgs>
	ComponentType* Entity::addComponent(ConstructorArgs&& ...args){
		if(isValid()){
			return manager->addComponent<ComponentType>(getID(), args...);
		} else{
			return nullptr;
		}
	}
	
	template<typename ComponentType>
	ComponentType* Entity::getComponent() const{
		if(isValid()){
			return manager->getComponent<ComponentType>(getID());
		} else{
			return nullptr;
		}
	}

	template<typename ComponentType>
	void Entity::removeComponent(){
		if(isValid()){
			manager->removeComponent<ComponentType>(getID());
		}
	}
}
