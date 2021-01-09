#include "Membrane/ViewportSurface.hpp"

using namespace garlic::clove;

namespace garlic::membrane {
    ViewportSurface::ViewportSurface()
        : keyboard{ keyboardDispatcher } 
        , mouse{ mouseDispatcher } {
        messageProxy = gcnew ViewportSurfaceMessageProxy(this);
    }

    MultiCastDelegate<void(vec2ui const &)> &ViewportSurface::onSurfaceResize() {
        return resizeDelegate;
    }

    void ViewportSurface::processInput(){

    };

    void ViewportSurface::setSize(clove::vec2i size) {
        this->size = size;
        resizeDelegate.broadcast(this->size);
    }

    vec2i ViewportSurface::getSize() const {
        return size;
    }

    Keyboard &ViewportSurface::getKeyboard() {
        return keyboard;
    }

    Mouse &ViewportSurface::getMouse() {
        return mouse;
    }
}