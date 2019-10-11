#include "Text.hpp"

namespace clv::ui{
	Text::Text(Font font)
		: font(std::move(font)){
	}

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