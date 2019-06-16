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

		ComponentType* operator->() const;
		ComponentType* get() const;
	};
}

#include "ComponentPtr.inl"