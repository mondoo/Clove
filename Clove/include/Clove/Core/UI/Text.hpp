#pragma once

#include "Clove/Core/UI/Font.hpp"

namespace clv::ui{
	class Text{
		//VARIABLES
	private:
		Font font;
		std::string text;

		//FUNCTIONS
	public:
		Text() = delete;
		Text(Font font);

		Text(const Text& other);
		Text(Text&& other);

		Text& operator=(const Text& other);
		Text& operator=(Text&& other);

		~Text();

		void setText(std::string text);
		void setSize(uint32 size);

		std::size_t getTextLength() const;
		Glyph getBufferForCharAt(size_t index) const;
	};
}