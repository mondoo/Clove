#pragma once

#include "Core/ECS/Component.hpp"

#include "Core/Utils/HashString.hpp"

namespace clv::gfx{
	class Sprite;
}

namespace clv::ecs::_2D{
	class SpriteComponent : public Component{
		friend class RenderSystem;

		//VARIABLES
	public:
		static constexpr ComponentID ID = HASH_CLASS(SpriteComponent);

	private:
		std::shared_ptr<gfx::Sprite> sprite;

		//FUNCTIONS
	public:
		SpriteComponent();
		SpriteComponent(const SpriteComponent& other) = delete;
		SpriteComponent(SpriteComponent&& other) noexcept;
		SpriteComponent& operator=(const SpriteComponent& other) = delete;
		SpriteComponent& operator=(SpriteComponent&& other) noexcept;
		virtual ~SpriteComponent();

		void setSprite(const std::shared_ptr<gfx::Sprite>& sprite);
	};
}

