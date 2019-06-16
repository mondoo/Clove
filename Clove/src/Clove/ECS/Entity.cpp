#include "clvpch.hpp"
#include "Entity.hpp"

namespace clv::ecs{
	Entity::Entity() = default;
	Entity::Entity(const Entity& other) = default;

	Entity::Entity(Entity&& other) noexcept = default;

	Entity& Entity::operator=(const Entity& other) = default;

	Entity& Entity::operator=(Entity&& other) noexcept = default;

	Entity::~Entity() = default;

	Entity::Entity(Manager* manager, EntityID entityID)
		: manager(manager)
		, entityID(entityID){
	}

	bool Entity::isValid() const{
		if(entityID == INVALID_ENTITY_ID){
			return false;
		}

		const auto it = manager->components.find(entityID);
		return it != manager->components.end();
	}

	EntityID Entity::getID(){
		return entityID;
	}
}