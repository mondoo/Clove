#include "Bulb/UI/Text.hpp"

#include "Bulb/Rendering/Renderables/Sprite.hpp"
#include "Bulb/Rendering/Renderer3D.hpp"

//Temp?
#include "Clove/Graphics/GraphicsTypes.hpp"
#include "Clove/Platform/Window.hpp"
//#include "Bulb/Rendering/Renderables/Sprite.hpp"

using namespace clv;
using namespace clv::gfx;

namespace blb::ui {
	Text::Text(rnd::Font font, std::shared_ptr<GraphicsFactory> graphicsFactory)
		: graphicsFactory(std::move(graphicsFactory))
		, font(std::move(font)) {
	}

	Text::Text(const Text& other) = default;

	Text::Text(Text&& other) = default;

	Text& Text::operator=(const Text& other) = default;

	Text& Text::operator=(Text&& other) = default;

	Text::~Text() = default;

	void Text::draw(rnd::Renderer3D& renderer, const clv::mth::vec2f& drawSpace) {
		const mth::vec2f screenHalfSize{ static_cast<float>(drawSpace.x) / 2.0f, static_cast<float>(drawSpace.y) / 2.0f };

		mth::vec2f cursorPos = { position.x - screenHalfSize.x, -position.y + screenHalfSize.y };

		const mth::mat4f projection = mth::createOrthographicMatrix(-screenHalfSize.x, screenHalfSize.x, -screenHalfSize.y, screenHalfSize.y);

		for(size_t i = 0; i < getTextLength(); ++i) {
			const rnd::Glyph& glyph = getBufferForCharAt(i);

			//For spaces we just skip and proceed
			if(glyph.character != nullptr) {
				const float width = glyph.size.x;
				const float height = glyph.size.y;

				const float xpos = cursorPos.x + glyph.bearing.x;
				const float ypos = cursorPos.y - (height - glyph.bearing.y);

				mth::mat4f model = mth::mat4f(1.0f);
				model = mth::translate(mth::mat4f(1.0f), { xpos, ypos, 0.0f });
				model *= mth::scale(mth::mat4f(1.0f), { width, height, 0.0f });

				auto character = std::make_shared<rnd::Sprite>(glyph.character, graphicsFactory);
				character->getMaterialInstance().setData(BufferBindingPoint::BBP_2DData, projection * model, ShaderStage::Vertex);

				renderer.submitText(character);
			}

			cursorPos.x += glyph.advance.x;
		}
	}

	void Text::setText(std::string text) {
		this->text = std::move(text);
		isBufferDirty = true;
	}

	void Text::setFontSize(uint32_t size) {
		font.setFontSize(size);
		isBufferDirty = true;
	}

	std::size_t Text::getTextLength() const {
		return text.length();
	}

	const rnd::Glyph& Text::getBufferForCharAt(size_t index) {
		if(isBufferDirty) {
			buildGlyphs();
		}
		return characters[index];
	}

	void Text::buildGlyphs() {
		characters.clear();
		for(size_t i = 0; i < text.length(); ++i) {
			characters.emplace_back(font.getChar(text[i]));
		}
		isBufferDirty = false;
	}
}