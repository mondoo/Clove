#pragma once

#include "Clove/Core/ECS/Component.hpp"

namespace clv::gfx{
	class Sprite;
}

namespace clv::ecs::_2D{
	class RenderSystem;
}

namespace clv::ecs::ui{
	class WidgetComponent : public Component<WidgetComponent>{
		friend class _2D::RenderSystem;

		//VARIABLES
	private:
		std::shared_ptr<gfx::Sprite> sprite;

		//TODO: Current works exactly like the SpriteComponent 
		//but will later be used to handle user focus

		//FUNCTIONS
	public:
		WidgetComponent();

		WidgetComponent(const WidgetComponent& other) = delete;
		WidgetComponent(WidgetComponent&& other) noexcept;

		WidgetComponent& operator=(const WidgetComponent& other) = delete;
		WidgetComponent& operator=(WidgetComponent&& other) noexcept;

		virtual ~WidgetComponent();

		void setSprite(const std::shared_ptr<gfx::Sprite>& sprite);
	};
}