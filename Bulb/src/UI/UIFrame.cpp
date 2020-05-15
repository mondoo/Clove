#include "Bulb/UI/UIFrame.hpp"

#include "Bulb/UI/DrawableElement.hpp"
#include "Bulb/UI/InputElement.hpp"

namespace blb::ui {
	UIFrame::UIFrame(clv::mth::vec2i size)
		: size(std::move(size)) {
	}

	InputResponse UIFrame::propagateInput(const clv::InputEvent& inputEvent) {
		switch(inputEvent.eventType) {
			case clv::InputEventType::Keyboard:
				//TODO: Needs to check if the elemet has focus
				break;

			case clv::InputEventType::Mouse: {
				const auto& mouseEvent = std::get<clv::Mouse::Event>(inputEvent.event);
				for(auto& element : inputElements) {
					const ElementBounds bounds = element->getBounds();
					const auto [mx, my] = mouseEvent.getPos();

					if(mx >= bounds.start.x && mx <= bounds.end.x &&
						my >= bounds.start.y && my <= bounds.end.y) {
						if(element->onMouseEvent(mouseEvent) == InputResponse::Consumed) {
							return InputResponse::Consumed;
						}
					}
				}
			} break;
			default:
				break;
		}

		return InputResponse::Ignored;
	}

	void UIFrame::drawChildren(rnd::Renderer3D& renderer) {
		for(auto& element : drawables) {
			element->draw(renderer, size);
		}
	}
}