#pragma once

#include "Clove/ECS/Component.hpp"

#include "Clove/UI/Text.hpp"

namespace clv::ecs::ui{
	class TextComponent : public Component{
		friend class FontSystem;

		//VARIABLES
	public:
		static constexpr ComponentID ID = 0xdf019850; //VS Generated GUID

	private:
		clv::ui::Text text;

		//FUNCTIONS
	public:
		//TODO: ctor

		/*
		STEPS TODO NEXT:
		-Currently adding the text class to the textcomponent to link in font rendering to the ECS
		-The text class needs to be constructed with a font class but components can't use constructors
		-Going to refactor the ECS to be able to use those constructors
		-Then update this accordingly 
		*/

	//	void setFont(const std::string& path);
	//	void setText(const std::string& text);
	};
}