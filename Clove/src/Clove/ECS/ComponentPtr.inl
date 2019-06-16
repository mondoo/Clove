#include "Clove/ECS/Component.hpp"
#include "Clove/ECS/Manager.hpp"

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
		: manager(manager)
		, entityID(entityID){
	}

	template<typename ComponentType>
	bool ComponentPtr<ComponentType>::isValid() const{
		if(entityID == INVALID_ENTITY_ID){
			return false;
		}

		const auto it = manager->components.find(entityID);
		if(it == manager->components.end()){
			return false;
		}

		return manager->components[entityID].find(ComponentType::ID) != manager->components[entityID].end();
	}

	template<typename ComponentType>
	ComponentType* ComponentPtr<ComponentType>::operator->() const{
		return get();
	}

	template<typename ComponentType>
	ComponentType* ComponentPtr<ComponentType>::get() const{
		if(isValid()){
			return static_cast<ComponentType*>(manager->components[entityID][ComponentType::ID].get());
		} else{
			return nullptr;
		}
	}
}