#include "Tunic/Rendering/Renderables/Text.hpp"

using namespace clv;

namespace tnc::rnd{
	Text::Text(Font font)
		: font(std::move(font)){
	}

	Text::Text(const Text& other) = default;

	Text::Text(Text &&other) = default;

	Text& Text::operator=(const Text& other) = default;

	Text &Text::operator=(Text &&other) = default;

	Text::~Text() = default;

	void Text::setText(std::string text){
		this->text = std::move(text);
	}

	void Text::setSize(uint32 size){
		font.setSize(size);
	}

	std::size_t Text::getTextLength() const{
		return text.length();
	}

	Glyph Text::getBufferForCharAt(size_t index) const{
		return font.getChar(text[index]);
	}
}