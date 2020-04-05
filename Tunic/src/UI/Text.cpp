#include "Tunic/UI/Text.hpp"

using namespace clv;

namespace tnc::ui {
	Text::Text(rnd::Font font)
		: font(std::move(font)) {
	}

	Text::Text(const Text& other) = default;

	Text::Text(Text&& other) = default;

	Text& Text::operator=(const Text& other) = default;

	Text& Text::operator=(Text&& other) = default;

	Text::~Text() = default;

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