#pragma once

#include "Core/ECS/Component.hpp"

#include "Core/UI/Text.hpp"
#include "Core/Utils/HashString.hpp"

namespace clv::ecs::ui{
	class TextComponent : public Component{
		friend class TextSystem;

		//VARIABLES
	public:
		static constexpr ComponentID ID = HASH_CLASS(TextComponent);

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