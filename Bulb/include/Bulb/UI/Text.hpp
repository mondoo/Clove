#pragma once

#include "Bulb/UI/Widget.hpp"

#include "Bulb/Rendering/Renderables/Font.hpp"

namespace clv::gfx {
	class GrpahicsFactory;
}

namespace blb::ui {
	class Text : public Widget{
		//VARIABLES
	private:
		std::shared_ptr<clv::gfx::GraphicsFactory> graphicsFactory;

		rnd::Font font;
		std::string text;

		bool isBufferDirty = false;
		std::vector<rnd::Glyph> characters;

		//FUNCTIONS
	public:
		Text() = delete;
		Text(rnd::Font font, std::shared_ptr<clv::gfx::GraphicsFactory> graphicsFactory);

		Text(const Text& other);
		Text(Text&& other);

		Text& operator=(const Text& other);
		Text& operator=(Text&& other);

		~Text();

		void draw(rnd::Renderer2D& renderer, const clv::mth::vec2f& drawSpace) override;

		void setText(std::string text);
		void setFontSize(uint32_t size);

		std::size_t getTextLength() const;
		const rnd::Glyph& getBufferForCharAt(size_t index);

	private:
		void buildGlyphs();
	};
}