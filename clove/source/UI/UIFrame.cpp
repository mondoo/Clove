#include "Clove/UI/UIFrame.hpp"

#include "Clove/UI/DrawableElement.hpp"
#include "Clove/UI/InputElement.hpp"

namespace garlic::clove {
    UIFrame::UIFrame(vec2i size)
        : size(std::move(size)) {
    }

    InputResponse UIFrame::propagateInput(InputEvent const &inputEvent) {
        switch(inputEvent.eventType) {
            case InputEventType::Keyboard:
                //TODO: Needs to check if the elemet has focus
                break;

            case InputEventType::Mouse: {
                auto const &mouseEvent = std::get<Mouse::Event>(inputEvent.event);
                for(auto &element : inputElements) {
                    ElementBounds const bounds = element->getBounds();
                    auto const [mx, my]        = mouseEvent.getPos();

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

    void UIFrame::drawChildren() {
        for(auto &element : drawables) {
            element->draw(size);
        }
    }
}