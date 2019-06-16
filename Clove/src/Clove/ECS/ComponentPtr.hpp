#pragma once

#include "Clove/ECS/ECSTypes.hpp"

namespace clv::ecs{
	class Manager;
	class Component;

	class ComponentPtrBase{
		//VARIABLES
	protected:
		Manager* manager = nullptr;
		EntityID entityID = INVALID_ENTITY_ID;

		//FUNCTIONS
	public:
		ComponentPtrBase();
		ComponentPtrBase(const ComponentPtrBase& other);
		ComponentPtrBase(ComponentPtrBase&& other) noexcept;
		ComponentPtrBase& operator=(const ComponentPtrBase& other);
		ComponentPtrBase& operator=(ComponentPtrBase&& other) noexcept;
		~ComponentPtrBase();

		ComponentPtrBase(Manager* manager, EntityID entityID);

	protected:
		bool isValid(ComponentID ID) const;
		Component* getComponent(ComponentID ID);
	};

	template<typename ComponentType>
	class ComponentPtr : public ComponentPtrBase{
		//VARIABLES

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
			if(ComponentPtrBase::isValid(ComponentType::ID)){
				static_cast<ComponentType*>(getComponent(ComponentType::ID));
			} else{
				return nullptr;
			}
		}
	};
}

#include "ComponentPtr.inl"