#pragma once

//#include "Clove/Scene/SceneNode.hpp"

namespace clv::ecs{
	class Entity/* : public scene::SceneNode*/{
		//VARIABLES
	private:


		//FUNCTIONS
	public:
		Entity();
		Entity(const Entity& other);
		Entity(Entity&& other) noexcept;
		Entity& operator=(const Entity& other);
		Entity& operator=(Entity&& other) noexcept;
		virtual ~Entity();

		/*virtual void update(float deltaSeconds) override;*/
	};
}
