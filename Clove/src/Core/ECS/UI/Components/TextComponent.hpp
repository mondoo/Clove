#pragma once

#include "Core/ECS/Component.hpp"

#include "Core/UI/Text.hpp"

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
		TextComponent(const TextComponent& other) = delete;
		TextComponent(TextComponent&& other);
		TextComponent& operator=(const TextComponent& other) = delete;
		TextComponent& operator=(TextComponent&& other);
		~TextComponent();

		TextComponent(clv::ui::Text text);

		void setText(std::string text);
		void setSize(uint32 size);
	};
}