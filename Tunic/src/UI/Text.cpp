#include "Tunic/UI/Text.hpp"

#include "Tunic/Rendering/Renderables/Sprite.hpp"
#include "Tunic/Rendering/Renderer2D.hpp"

//Temp?
#include "Clove/Graphics/Core/GraphicsTypes.hpp"
#include "Clove/Platform/Core/Window.hpp"
#include "Tunic/Application.hpp"
//#include "Tunic/Rendering/Renderables/Sprite.hpp"

using namespace clv;
using namespace clv::gfx;

namespace tnc::ui {
	Text::Text(rnd::Font font)
		: font(std::move(font)) {
	}

	Text::Text(const Text& other) = default;

	Text::Text(Text&& other) = default;

	Text& Text::operator=(const Text& other) = default;

	Text& Text::operator=(Text&& other) = default;

	Text::~Text() = default;

	void Text::draw(rnd::Renderer2D& renderer) {
		//Ported from 2D::RenderSystem
		//Screen size
		const mth::vec2i screenSize = tnc::Application::get().getMainWindow().getSize();
		const mth::vec2f screenHalfSize{ static_cast<float>(screenSize.x) / 2.0f, static_cast<float>(screenSize.y) / 2.0f };

		const float scaleFactor = 40.0f; //How big we want our 2D elements to be

		//We want our UI to translate pixel perfect, so we create a projection that's the size of the screen and then scale the model
		const mth::mat4f uiProjection = mth::createOrthographicMatrix(-screenHalfSize.x, screenHalfSize.x, -screenHalfSize.y, screenHalfSize.y);
		const mth::mat4f uiScale = mth::scale(mth::mat4f{ 1.0f }, mth::vec3f{ scaleFactor });

		//Rendering
		GraphicsFactory& graphicsFactory = Application::get().getGraphicsFactory();

		/*mth::vec2f offset{};
		const mth::vec2f anchor = transform->getAnchor();

		if(ui::TransformComponent* parent = transform->getParent()) {
			const mth::vec2f parentScale = parent->getScale();
			offset.x = anchor.x * parentScale.x;
			offset.y = anchor.y * parentScale.y;
		} else {
			offset.x = anchor.x * screenSize.x;
			offset.y = anchor.y * screenSize.y;
		}*/

		mth::vec2f cursorPos = position;
		/*cursorPos.x -= (screenHalfSize.x - offset.x);
		cursorPos.y += (screenHalfSize.y + offset.y);*/

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

				auto character = std::make_shared<rnd::Sprite>(glyph.character);
				character->getMaterialInstance().setData(BufferBindingPoint::BBP_2DData, uiProjection * model, ShaderStage::Vertex);

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