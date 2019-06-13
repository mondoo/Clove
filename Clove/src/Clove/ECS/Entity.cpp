#include "clvpch.hpp"
#include "Entity.hpp"

#include "Clove/ECS/Component.hpp"

namespace clv::ecs{
	Entity::Entity(Entity&& other) noexcept{
		ID = other.ID;
		components = std::move(other.components);
	}

	Entity& Entity::operator=(Entity&& other) noexcept{
		ID = other.ID;
		components = std::move(other.components);

		return *this;
	}

	Entity::~Entity() = default;

	Entity::Entity(EntityID ID)
		: ID(ID){
	}

	EntityID Entity::getID() const{
		return ID;
	}

	const std::unordered_map<ComponentID, std::unique_ptr<Component>>& Entity::getComponents() const{
		return components;
	}
}
