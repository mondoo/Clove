#include "clvpch.hpp"
#include "Entity.hpp"

namespace clv::ecs{
	Entity::Entity() = default;
	Entity::Entity(const Entity& other) = default;

	Entity::Entity(Entity&& other) noexcept{
		entityID = other.entityID;

		onComponentRequestedDelegate = std::move(other.onComponentRequestedDelegate);
		isEntityIdValidDelegate = std::move(other.isEntityIdValidDelegate);
	}

	Entity& Entity::operator=(const Entity& other) = default;

	Entity& Entity::operator=(Entity&& other) noexcept{
		entityID = other.entityID;

		onComponentRequestedDelegate = std::move(other.onComponentRequestedDelegate);
		isEntityIdValidDelegate = std::move(other.isEntityIdValidDelegate);

		return *this;
	}

	Entity::~Entity() = default;

	Entity::Entity(EntityID entityID)
		: entityID(entityID){
	}

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