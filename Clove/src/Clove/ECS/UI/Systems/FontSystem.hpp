#pragma once

#include "Clove/ECS/System.hpp"

#include "Clove/ECS/UI/Components/FontComponent.hpp"
#include "Clove/ECS/2D/Components/TransformComponent.hpp" //TODO: This will probably have to be a UI position component


//Temp
#include "Clove/Graphics/Sprite.hpp"

namespace clv::ecs::ui{
	class FontSystem : public System<FontComponent, d2::TransformComponent>{
		//VARIABLES
	public:
		//TEMP:
		std::shared_ptr<gfx::Sprite> sprite;

		//FUNCTIONS
	public:
		//TODO: The rest
		FontSystem();

		virtual void update(utl::DeltaTime deltaTime) override;
	};
}