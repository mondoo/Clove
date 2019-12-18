#pragma once

#include "Clove/Core/ECS/Component.hpp"

namespace clv::gfx{
	class Sprite;
}

namespace clv::ecs::_2D{
	class SpriteComponent : public Component<SpriteComponent>{
		friend class RenderSystem;

		//VARIABLES
	private:
		std::shared_ptr<gfx::Sprite> sprite;

		//FUNCTIONS
	public:
		SpriteComponent();
		SpriteComponent(const std::shared_ptr<gfx::Sprite>& sprite);

		SpriteComponent(const SpriteComponent& other);
		SpriteComponent(SpriteComponent&& other) noexcept;

		SpriteComponent& operator=(const SpriteComponent& other);
		SpriteComponent& operator=(SpriteComponent&& other) noexcept;

		virtual ~SpriteComponent();

		void setSprite(const std::shared_ptr<gfx::Sprite>& sprite);
	};
}

