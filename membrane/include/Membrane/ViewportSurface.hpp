#pragma once

#include <Clove/Platform/Input/Keyboard.hpp>
#include <Clove/Platform/Input/Mouse.hpp>
#include <Clove/Surface.hpp>
#include <msclr/gcroot.h>

namespace garlic::membrane {
    /**
     * @brief A surface backed by an editor viewport.
     */
    class ViewportSurface : public clove::Surface {
        friend ref class ViewportSurfaceMessageProxy;

        //VARIABLES
    private:
        msclr::gcroot<ViewportSurfaceMessageProxy ^> messageProxy;

        clove::MultiCastDelegate<void(clove::vec2ui const &)> resizeDelegate{};

        clove::vec2i size{};

        clove::Keyboard keyboard;
        clove::Mouse mouse;

        clove::Keyboard::Dispatcher keyboardDispatcher{};
        clove::Mouse::Dispatcher mouseDispatcher{};

        //FUNCTIONS
    public:
        //TODO: Ctors
        ViewportSurface();

        clove::MultiCastDelegate<void(clove::vec2ui const &)> &onSurfaceResize() override;

        void processInput() override;

        void setSize(clove::vec2i size);
        clove::vec2i getSize() const override;

        clove::Keyboard &getKeyboard() override;
        clove::Mouse &getMouse() override;
    };
}