#pragma once

#include "Clove/ECS/Components/Renderable2DComponent.hpp"

namespace clv::ecs{
	class SpriteComponent : public Renderable2DComponent{
		//VARIABLES
	private:
		
		//FUNCTIONS
	public:
		SpriteComponent();
		SpriteComponent(const SpriteComponent& other) = delete;
		SpriteComponent(SpriteComponent&& other) noexcept;
		SpriteComponent& operator=(const SpriteComponent& other) = delete;
		SpriteComponent& operator=(SpriteComponent&& other) noexcept;
		virtual ~SpriteComponent();

		void setTexture(const std::string& path);
	};
}

