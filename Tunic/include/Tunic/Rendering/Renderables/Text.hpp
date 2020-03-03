#pragma once

#include "Tunic/Rendering/Renderables/Font.hpp"

namespace tnc::rnd {
	class Text {
		//VARIABLES
	private:
		Font font;
		std::string text;

		bool isBufferDity = false;
		std::vector<Glyph> characters;

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
		void setFontSize(uint32_t size);

		std::size_t getTextLength() const;
		const Glyph& getBufferForCharAt(size_t index);

	private:
		void buildGlyphs();
	};
}