#include "Membrane/ViewportSurface.hpp"

using namespace garlic::clove;

namespace garlic::membrane {
    MultiCastDelegate<void(vec2ui const &)> &ViewportSurface::onSurfaceResize() {
        return resizeDelegate;
    }

    void ViewportSurface::processInput(){

    };

    vec2i ViewportSurface::getSize() const{
        return { 1.0f, 1.0f };
    }

    Keyboard &ViewportSurface::getKeyboard() {
        return keyboard;
    }

    Mouse &ViewportSurface::getMouse() {
        return mouse;
    }
}