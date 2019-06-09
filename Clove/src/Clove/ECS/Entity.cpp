#include "clvpch.hpp"
#include "Entity.hpp"

namespace clv::ecs{
	Entity::Entity(const Entity& other) = default;

	Entity::Entity(Entity&& other) noexcept{
		id = other.id;
		components = std::move(other.components);
	}

	Entity& Entity::operator=(const Entity& other) = default;

	Entity& Entity::operator=(Entity&& other) noexcept = default;

	Entity::~Entity() = default;

	Entity::Entity(EntityID id)
		: id(id){

	}

	EntityID Entity::getID() const{
		return id;
	}

	const std::unordered_map<ComponentID, Component*>& Entity::getComponents() const{
		return components;
	}
}
