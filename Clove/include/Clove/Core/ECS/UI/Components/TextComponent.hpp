#pragma once

#include "Clove/Core/ECS/Component.hpp"

#include "Clove/Core/UI/Text.hpp"

namespace clv::ecs::_2D{
	class RenderSystem;
}

namespace clv::ecs::ui{
	class TextComponent : public Component<TextComponent>{
		friend class _2D::RenderSystem;

		//VARIABLES
	private:
		clv::ui::Text text;

		//FUNCTIONS
	public:
		TextComponent() = delete;
		TextComponent(clv::ui::Text text);

		TextComponent(const TextComponent& other);
		TextComponent(TextComponent&& other);

		TextComponent& operator=(const TextComponent& other);
		TextComponent& operator=(TextComponent&& other);

		virtual ~TextComponent();

		void setText(std::string text);
		void setSize(uint32 size);
	};
}