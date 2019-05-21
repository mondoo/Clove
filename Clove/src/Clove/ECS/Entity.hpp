#pragma once

#include "Clove/Scene/SceneNode.hpp"

namespace clv::gfx{
	class Mesh;
}

namespace clv::ecs{
	class Entity : public scene::SceneNode{
		//VARIABLES
		//Temp: just giving them all a mesh until this gets set up
	public:
		std::shared_ptr<gfx::Mesh> mesh;
		//

		//FUNCTIONS
	public:
		Entity();
		Entity(const Entity& other);
		Entity(Entity&& other) noexcept;
		Entity& operator=(const Entity& other);
		Entity& operator=(Entity&& other) noexcept;
		virtual ~Entity();

		virtual void update(float deltaSeconds) override;
	};
}
