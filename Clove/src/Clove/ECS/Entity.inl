#include "Clove/ECS/Manager.hpp"

namespace clv::ecs{
	template<typename ComponentType>
	ComponentType* Entity::getComponent() const{
		if(isValid()){
			return static_cast<ComponentType*>(manager->components[entityID][ComponentType::ID].get());
		}
		return nullptr;
	}
}