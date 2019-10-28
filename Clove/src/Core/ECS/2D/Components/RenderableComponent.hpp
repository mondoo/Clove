#pragma once

#include "Core/ECS/Component.hpp"

#include "Core/Graphics/Renderer.hpp"

namespace clv::gfx{
	class Sprite;
}

namespace clv::ecs::_2D{
	class RenderableComponent : public Component{
		friend class RenderSystem;

		//VARIABLES
	public:
		static constexpr ComponentID ID = 0x28fcf790; //VS Generated GUID

	private:
		std::shared_ptr<gfx::Sprite> sprite;

		//FUNCTIONS
	public:
		RenderableComponent();
		RenderableComponent(const RenderableComponent& other) = delete;
		RenderableComponent(RenderableComponent&& other) noexcept;
		RenderableComponent& operator=(const RenderableComponent& other) = delete;
		RenderableComponent& operator=(RenderableComponent&& other) noexcept;
		virtual ~RenderableComponent();

		void setSprite(const std::shared_ptr<gfx::Sprite>& sprite);
	};
}

