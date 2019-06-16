#pragma once

#include "Clove/ECS/ECSTypes.hpp"
#include "Clove/ECS/ComponentPtr.hpp"

namespace clv::ecs{
	class Manager;

	class EntityPtr{
		//VARIABLES
	private:
		Manager* manager = nullptr;
		EntityID entityID = INVALID_ENTITY_ID;

		//FUNCTIONS
	public:
		EntityPtr();
		EntityPtr(const EntityPtr& other);
		EntityPtr(EntityPtr&& other) noexcept;
		EntityPtr& operator=(const EntityPtr& other);
		EntityPtr& operator=(EntityPtr&& other) noexcept;
		~EntityPtr();

		EntityPtr(Manager* manager, EntityID entityID);

		bool isValid() const;

		EntityID getID();

		template<typename ComponentType>
		ComponentPtr<ComponentType> getComponent() const;
	};
}

#include "EntityPtr.inl"