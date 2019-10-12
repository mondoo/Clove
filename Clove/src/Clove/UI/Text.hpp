#pragma once

#include "Clove/UI/Font.hpp"

namespace clv::ui{
	class Text{
		//VARIABLES
	private:
		Font font;
		std::string text;

		//FUNCTIONS
	public:
		Text() = delete;
		Text(const Text& other) = delete;
		Text(Text&& other);
		Text& operator=(const Text& other) = delete;
		Text& operator=(Text&& other);
		~Text();

		Text(Font font);

		void setText(std::string text);
		void setSize(uint32 size);

		std::size_t getTextLength() const;
		Glyph getBufferForCharAt(size_t index) const;
	};
}