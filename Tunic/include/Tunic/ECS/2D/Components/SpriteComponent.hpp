#pragma once

#include "Tunic/ECS/Core/Component.hpp"

namespace tnc::rnd{
	class Sprite;
}

namespace tnc::ecs::_2D{
	class SpriteComponent : public Component<SpriteComponent>{
		friend class RenderSystem;

		//VARIABLES
	private:
		std::shared_ptr<rnd::Sprite> sprite;

		//FUNCTIONS
	public:
		SpriteComponent();
		SpriteComponent(const std::shared_ptr<rnd::Sprite>& sprite);

		SpriteComponent(const SpriteComponent& other);
		SpriteComponent(SpriteComponent&& other) noexcept;

		SpriteComponent& operator=(const SpriteComponent& other);
		SpriteComponent& operator=(SpriteComponent&& other) noexcept;

		virtual ~SpriteComponent();

		void setSprite(const std::shared_ptr<rnd::Sprite>& sprite);
	};
}

