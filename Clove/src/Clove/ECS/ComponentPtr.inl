#include "Clove/ECS/Component.hpp"

namespace clv::ecs{
	template<typename ComponentType>
	ComponentPtr<ComponentType>::ComponentPtr() = default;

	template<typename ComponentType>
	ComponentPtr<ComponentType>::ComponentPtr(const ComponentPtr& other) = default;

	template<typename ComponentType>
	ComponentPtr<ComponentType>::ComponentPtr(ComponentPtr&& other) noexcept = default;

	template<typename ComponentType>
	ComponentPtr<ComponentType>& ComponentPtr<ComponentType>::operator=(const ComponentPtr& other) = default;

	template<typename ComponentType>
	ComponentPtr<ComponentType>& ComponentPtr<ComponentType>::operator=(ComponentPtr&& other) noexcept = default;

	template<typename ComponentType>
	ComponentPtr<ComponentType>::~ComponentPtr() = default;

	template<typename ComponentType>
	ComponentPtr<ComponentType>::ComponentPtr(Manager* manager, EntityID entityID)
		: ComponentPtrBase(manager, entityID){
	}

	template<typename ComponentType>
	bool ComponentPtr<ComponentType>::isValid() const{
		return ComponentPtrBase::isValid(ComponentType::ID);
	}

	template<typename ComponentType>
	ComponentType* ComponentPtr<ComponentType>::operator->(){
		if(isValid()){
			static_cast<ComponentType*>(getComponent(ComponentType::ID));
		} else{
			return nullptr;
		}
	}
}