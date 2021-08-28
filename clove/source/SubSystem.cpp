#include "Clove/SubSystem.hpp"

namespace clove {
    SubSystem::SubSystem(std::string name)
        : debugName(std::move(name)) {
    }

    InputResponse SubSystem::onInputEvent(InputEvent const &inputEvent) {
        return InputResponse::Ignored;
    }
}
