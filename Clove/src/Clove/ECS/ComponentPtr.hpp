#pragma once

#include "Clove/ECS/ECSTypes.hpp"

namespace clv::ecs{
	class Manager;
	class Component;

	template<typename ComponentType>
	class ComponentPtr{
		//VARIABLES
	private:
		Manager* manager = nullptr;
		EntityID entityID = INVALID_ENTITY_ID;

		//FUNCTIONS
	public:
		ComponentPtr();
		ComponentPtr(const ComponentPtr& other);
		ComponentPtr(ComponentPtr&& other) noexcept;
		ComponentPtr<ComponentType>& operator=(const ComponentPtr& other);
		ComponentPtr<ComponentType>& operator=(ComponentPtr&& other) noexcept;
		~ComponentPtr();

		ComponentPtr(Manager* manager, EntityID entityID);

		bool isValid() const;

		ComponentType* operator->();

		//Temp: This is for the add child thing but I want avoid people having access to the pointers underneath
		ComponentType* get(){
			if(isValid()){
				static_cast<ComponentType*>(getComponent());
			} else{
				return nullptr;
			}
		}

	protected:
		ComponentType* getComponent() const;
	};
}

#include "ComponentPtr.inl"