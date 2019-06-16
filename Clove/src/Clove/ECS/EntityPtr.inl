#include "Clove/ECS/Manager.hpp"

namespace clv::ecs{
	template<typename ComponentType>
	ComponentPtr<ComponentType> EntityPtr::getComponent() const{
		if(isValid()){
			if(manager->components[entityID][ComponentType::ID] != nullptr){
				return { manager, entityID };
			}
		}

		return {};
	}
}