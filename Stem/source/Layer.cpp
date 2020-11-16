#include "Stem/Layer.hpp"

namespace garlic::inline stem {
    Layer::Layer(std::string name)
        : debugName(std::move(name)) {
    }

    InputResponse Layer::onInputEvent(InputEvent const &inputEvent) {
        return InputResponse::Ignored;
    }
}
