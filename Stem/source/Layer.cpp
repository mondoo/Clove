#include "Stem/Layer.hpp"

namespace garlic::inline stem {
    Layer::Layer(std::string name)
        : debugName(std::move(name)) {
    }

    blb::InputResponse Layer::onInputEvent(clv::InputEvent const &inputEvent) {
        return blb::InputResponse::Ignored;
    }
}
