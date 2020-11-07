#pragma once

#include "Bulb/UI/DrawableElement.hpp"

#include "Bulb/Rendering/Renderables/Font.hpp"

namespace clv::gfx {
	class GrpahicsFactory;
}

namespace blb::ui {
	class Text : public DrawableElement {
		//VARIABLES
	private:
		std::shared_ptr<clv::gfx::GraphicsFactory> graphicsFactory;

		rnd::Font font;
		std::string text;

		bool isBufferDirty = false;
		std::vector<rnd::Glyph> characters;

		clv::mth::vec2f position{ 0.0f, 0.0f };
		float rotation{ 0.0f };
		clv::mth::vec2f scale{ 1.0f, 1.0f };

		//FUNCTIONS
	public:
		Text() = delete;
		Text(rnd::Font font, std::shared_ptr<clv::gfx::GraphicsFactory> graphicsFactory);

		Text(const Text& other);
		Text(Text&& other);

		Text& operator=(const Text& other);
		Text& operator=(Text&& other);

		~Text();

		//TODO: Interface for these?
		void setPosition(clv::mth::vec2f position);
		void setRotation(float rotation);
		void setScale(clv::mth::vec2f scale);

		const clv::mth::vec2f& getPosition() const;
		float getRotation() const;
		const clv::mth::vec2f& getScale() const;

        void draw(rnd::ForwardRenderer3D &renderer, const clv::mth::vec2f &drawSpace) override;

        void setText(std::string text);
		void setFontSize(uint32_t size);

		std::size_t getTextLength() const;
		const rnd::Glyph& getBufferForCharAt(size_t index);

	private:
		void buildGlyphs();
	};
}