#include "Tunic/ECS/Entity.hpp"

namespace tnc::ecs{
	Entity::Entity() = default;

	Entity::Entity(EntityID entityID, World* world)
		: entityID(entityID)
		, world(world) {
	}

	Entity::Entity(const Entity& other){
		Entity clonedOther = other.world->cloneEntitiesComponents(other.getID());
		world = clonedOther.world;
		entityID = clonedOther.entityID;
	}

	Entity::Entity(Entity&& other) noexcept = default;

	Entity& Entity::operator=(const Entity& other){
		Entity clonedOther = other.world->cloneEntitiesComponents(other.getID());
		world = clonedOther.world;
		entityID = clonedOther.entityID;

		return *this;
	}

	Entity& Entity::operator=(Entity&& other) noexcept = default;

	Entity::~Entity() = default;

	bool Entity::isValid() const{
		return world != nullptr && world->isEntityValid(entityID);
	}

	EntityID Entity::getID() const{
		return entityID;
	}

	Entity::operator EntityID() const {
		return entityID;
	}
}