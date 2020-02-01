#pragma once

#include "Tunic/ECS/Core/Component.hpp"

#include "Tunic/Rendering/Renderables/Text.hpp"

namespace tnc::ecs::_2D{
	class RenderSystem;
}

namespace tnc::ecs::ui{
	class TextComponent : public Component<TextComponent>{
		friend class _2D::RenderSystem;

		//VARIABLES
	private:
		rnd::Text text;

		//FUNCTIONS
	public:
		TextComponent() = delete;
		TextComponent(rnd::Text text);

		TextComponent(const TextComponent& other);
		TextComponent(TextComponent&& other);

		TextComponent& operator=(const TextComponent& other);
		TextComponent& operator=(TextComponent&& other);

		virtual ~TextComponent();

		void setText(std::string text);
		void setSize(uint32_t size);
	};
}