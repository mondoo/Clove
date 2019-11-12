#include "Entity.hpp"

namespace clv::ecs{
	Entity::Entity() = default;

	Entity::Entity(EntityID entityID)
			: entityID(entityID){
	}

	Entity::Entity(const Entity& other) = default;

	Entity::Entity(Entity&& other) noexcept{
		onComponentCreated = std::move(other.onComponentCreated);
		onComponentRequestedDelegate = std::move(other.onComponentRequestedDelegate);
		isEntityIdValidDelegate = std::move(other.isEntityIdValidDelegate);
		
		entityID = std::move(other.entityID);
	}

	Entity& Entity::operator=(const Entity& other) = default;

	Entity& Entity::operator=(Entity&& other) noexcept{
		onComponentCreated = std::move(other.onComponentCreated);
		onComponentRequestedDelegate = std::move(other.onComponentRequestedDelegate);
		isEntityIdValidDelegate = std::move(other.isEntityIdValidDelegate);

		entityID = std::move(other.entityID);

		return *this;
	}

	Entity::~Entity() = default;

	bool Entity::isValid() const{
		if(entityID == INVALID_ENTITY_ID){
			return false;
		}

		return isEntityIdValidDelegate.broadcast(entityID);
	}

	EntityID Entity::getID() const{
		return entityID;
	}
}