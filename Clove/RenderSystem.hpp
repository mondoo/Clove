#pragma once

#include "Clove/ECS/System.hpp"

#include "Clove/ECS/Components/TransformComponent.hpp"
//TODO: renderable

namespace clv::ecs{
										//vvvv components the thing needs
	class RenderSystem : public System<TransformComponent /*, Renderable etc.*/>{
		//VARIABLES

		//FUNCTIONS
	public:
		virtual void update(float deltaTime) override;
	};
}
