#pragma once

#include "Clove/ECS/Component.hpp"

#include "Clove/UI/Text.hpp"

namespace clv::ecs::ui{
	class TextComponent : public Component{
		friend class TextSystem;

		//VARIABLES
	public:
		static constexpr ComponentID ID = 0xdf019850; //VS Generated GUID

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