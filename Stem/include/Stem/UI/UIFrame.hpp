#pragma once

#include "Bulb/InputResponse.hpp"

#include <Clove/Input/InputEvent.hpp>

namespace garlic::inline stem {
    class ForwardRenderer3D;
}

namespace garlic::inline stem {
    class DrawableElement;
    class InputElement;
}

namespace garlic::inline stem {
    class UIFrame {
        //VARIABLES
    private:
        //SharedPtr for now, good candidate for an allocator to get cache friendly
        std::vector<std::shared_ptr<DrawableElement>> drawables;
        std::vector<std::shared_ptr<InputElement>> inputElements;

        clv::mth::vec2i size;

        //FUNCTIONS
    public:
        UIFrame() = delete;
        UIFrame(clv::mth::vec2i size);

        blb::InputResponse propagateInput(clv::InputEvent const &inputEvent);

        void drawChildren(ForwardRenderer3D &renderer);

        template<typename Element>
        void addElement(std::shared_ptr<Element> element) {
            if constexpr(std::is_base_of_v<DrawableElement, Element>) {
                drawables.push_back(element);
            }
            if constexpr(std::is_base_of_v<InputElement, Element>) {
                inputElements.push_back(element);
            }
        }
    };
}