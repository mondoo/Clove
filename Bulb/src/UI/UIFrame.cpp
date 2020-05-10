#include "Bulb/UI/UIFrame.hpp"

#include "Bulb/UI/DrawableElement.hpp"
#include "Bulb/UI/InputElement.hpp"

namespace blb::ui {
	UIFrame::UIFrame(clv::mth::vec2i size) 
		: size(std::move(size)){
	}

	InputResponse UIFrame::propogateInput(const clv::InputEvent& event) {
		for(auto& element : inputElements) {
			//Key - TODO: Needs to check if the elemet has focus
			/*if(event.keyboardEvent) {
				if(element->onKeyEvent(*event.keyboardEvent) == InputResponse::Consumed) {
					return InputResponse::Consumed;
				}
			}*/

			//Mouse
			if(event.mouseEvent) {
				const ElementBounds bounds = element->getBounds();
				const auto [mx, my] = event.mouseEvent->getPos();

				if(mx >= bounds.start.x && mx <= bounds.end.x && 
					my >= bounds.start.y && my <= bounds.end.y) {
					if(element->onMouseEvent(*event.mouseEvent) == InputResponse::Consumed) {
						return InputResponse::Consumed;
					}
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