#pragma once

#include "Clove/ECS/Component.hpp"

#include "Clove/UI/Font.hpp"

namespace clv::ecs::ui{
	class TextComponent : public Component{
		friend class FontSystem;

		//VARIABLES
	public:
		static constexpr ComponentID ID = 0xdf019850; //VS Generated GUID

	private:
		/*
		holds the font, holds the string?
		*/

		//Temp public
	public:
		clv::ui::Font font;
		std::string text;

		//FUNCTIONS
	public:
		//TODO: ctor
	};
}