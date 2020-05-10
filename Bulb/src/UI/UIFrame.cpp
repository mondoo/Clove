#include "Bulb/UI/UIFrame.hpp"

#include "Bulb/UI/DrawableElement.hpp"
#include "Bulb/UI/InputElement.hpp"

namespace blb::ui {
	UIFrame::UIFrame(clv::mth::vec2i size) 
		: size(std::move(size)){
	}

	InputResponse UIFrame::propogateInput(const clv::InputEvent& event) {
		for(auto& element : inputElements) {
			//Key
			if(event.keyboardEvent) {
				if(element->onKeyEvent(*event.keyboardEvent) == InputResponse::Consumed) {
					return InputResponse::Consumed;
				}
			}

			//Mouse
			if(event.mouseEvent) {
				if(element->onMouseEvent(*event.mouseEvent) == InputResponse::Consumed) {
					return InputResponse::Consumed;
				}
			}
		}

		return InputResponse::Ignored;
	}

	void UIFrame::drawChildren(rnd::Renderer3D& renderer) {
		for(auto& element : drawables) {
			element->draw(renderer, size);
		}
	}
}