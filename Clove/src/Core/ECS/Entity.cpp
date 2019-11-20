#include "Entity.hpp"

namespace clv::ecs{
	Entity::Entity() = default;

	Entity::Entity(EntityID entityID, Manager* manager)
		: entityID(entityID), manager(manager){
	}

	Entity::Entity(const Entity& other) = default;

	Entity::Entity(Entity&& other) noexcept = default;

	Entity& Entity::operator=(const Entity& other) = default;

	Entity& Entity::operator=(Entity&& other) noexcept = default;

	Entity::~Entity() = default;

	bool Entity::isValid() const{
		return entityID != INVALID_ENTITY_ID;
	}

	EntityID Entity::getID() const{
		return entityID;
	}
}