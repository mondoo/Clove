#pragma once

#include "Core/ECS/System.hpp"

#include "Core/ECS/UI/Components/TextComponent.hpp"
#include "Core/ECS/2D/Components/TransformComponent.hpp" //TODO: This will probably have to be a UI position component

namespace clv::ecs::ui{
	class TextSystem : public System<TextComponent, _2D::TransformComponent>{
		//FUNCTIONS
	public:
		//TODO: The rest
		TextSystem();
		~TextSystem();

		virtual void update(utl::DeltaTime deltaTime) override;
	};
}