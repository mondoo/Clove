#include "Core/ECS/Manager.hpp"

namespace clv::ecs{
	template<typename ComponentType, typename ...ConstructorArgs>
	ComponentType* Entity::addComponent(ConstructorArgs&& ...args){
		if(isValid()){
			return manager->componentManager.getComponentContainer<ComponentType>()->addComponent(getID(), args...);
		} else{
			return nullptr;
		}
	}
	
	template<typename ComponentType>
	ComponentType* Entity::getComponent() const{
		if(isValid()){
			return manager->componentManager.getComponentContainer<ComponentType>()->getComponent(getID());
		} else{
			return nullptr;
		}
	}
}