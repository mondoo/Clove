#include "Tunic/ECS/Core/Entity.hpp"

namespace tnc::ecs{
	Entity::Entity() = default;

	Entity::Entity(EntityID entityID, World* manager)
		: entityID(entityID), manager(manager){
	}

	Entity::Entity(const Entity& other){
		Entity clonedOther = other.manager->cloneEntitiesComponents(other.getID());
		manager = clonedOther.manager;
		entityID = clonedOther.entityID;
	}

	Entity::Entity(Entity&& other) noexcept = default;

	Entity& Entity::operator=(const Entity& other){
		Entity clonedOther = other.manager->cloneEntitiesComponents(other.getID());
		manager = clonedOther.manager;
		entityID = clonedOther.entityID;

		return *this;
	}

	Entity& Entity::operator=(Entity&& other) noexcept = default;

	Entity::~Entity() = default;

	bool Entity::isValid() const{
		return entityID != INVALID_ENTITY_ID;
	}

	EntityID Entity::getID() const{
		return entityID;
	}
}