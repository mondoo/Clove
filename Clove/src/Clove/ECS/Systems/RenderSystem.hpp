#pragma once

#include "Clove/ECS/System.hpp"

#include "Clove/ECS/Components/TransformComponent.hpp"
#include "Clove/ECS/Components/RenderableComponent.hpp"

//TODO: Init renderer

namespace clv::gfx{
	class Renderer;
}

namespace clv::ecs{
	class RenderSystem : public System<TransformComponent, RenderableComponent>{
		//VARIABLES
	private:
		std::unique_ptr<gfx::Renderer> renderer;

		//FUNCTIONS
	public:
		virtual void update(float deltaTime) override;
	};
}
