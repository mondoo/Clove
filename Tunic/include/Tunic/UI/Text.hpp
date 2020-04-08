#pragma once

#include "Tunic/UI/Widget.hpp"

#include "Tunic/Rendering/Renderables/Font.hpp"

namespace tnc::ui {
	class Text : public Widget{
		//VARIABLES
	private:
		rnd::Font font;
		std::string text;

		bool isBufferDirty = false;
		std::vector<rnd::Glyph> characters;

		//FUNCTIONS
	public:
		Text() = delete;
		Text(rnd::Font font);

		Text(const Text& other);
		Text(Text&& other);

		Text& operator=(const Text& other);
		Text& operator=(Text&& other);

		~Text();

		virtual void draw(rnd::Renderer2D& renderer) override;

		void setText(std::string text);
		void setFontSize(uint32_t size);

		std::size_t getTextLength() const;
		const rnd::Glyph& getBufferForCharAt(size_t index);

	private:
		void buildGlyphs();
	};
}