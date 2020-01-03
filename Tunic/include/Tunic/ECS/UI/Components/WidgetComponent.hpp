#pragma once

#include "Tunic/ECS/Core/Component.hpp"

namespace clv::gfx{
	class Sprite;
}

namespace tnc::ecs::_2D{
	class RenderSystem;
}

namespace tnc::ecs::ui{
	class WidgetComponent : public Component<WidgetComponent>{
		friend class _2D::RenderSystem;

		//VARIABLES
	private:
		std::shared_ptr<clv::gfx::Sprite> sprite;

		//TODO: Current works exactly like the SpriteComponent 
		//but will later be used to handle user focus

		//FUNCTIONS
	public:
		WidgetComponent();

		WidgetComponent(const WidgetComponent& other);
		WidgetComponent(WidgetComponent&& other) noexcept;

		WidgetComponent& operator=(const WidgetComponent& other);
		WidgetComponent& operator=(WidgetComponent&& other) noexcept;

		virtual ~WidgetComponent();

		void setSprite(const std::shared_ptr<clv::gfx::Sprite>& sprite);
	};
}