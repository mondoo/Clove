#include "clvpch.hpp"
#include "Entity.hpp"

namespace clv::ecs{
	Entity::Entity()
		/*: scene::SceneNode()*/{
	
	}

	Entity::Entity(const Entity& other) = default;

	Entity::Entity(Entity&& other) noexcept = default;

	Entity& Entity::operator=(const Entity& other) = default;

	Entity& Entity::operator=(Entity&& other) noexcept = default;

	Entity::~Entity() = default;

	/*void Entity::update(float deltaSeconds){
		scene::SceneNode::update(deltaSeconds);
	}*/
}
