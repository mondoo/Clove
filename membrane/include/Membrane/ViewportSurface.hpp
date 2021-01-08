#pragma once

#include <Clove/Platform/Input/Keyboard.hpp>
#include <Clove/Platform/Input/Mouse.hpp>
#include <Clove/Surface.hpp>

namespace garlic::membrane {
    /**
     * @brief A surface backed by an editor viewport.
     */
    class ViewportSurface : public clove::Surface {
        //VARIABLES
    private:
        clove::MultiCastDelegate<void(clove::vec2ui const &)> resizeDelegate{};

        clove::Keyboard keyboard{};
        clove::Mouse mouse{};

        //FUNCTIONS
    public:
        clove::MultiCastDelegate<void(clove::vec2ui const &)> &onSurfaceResize() override;

        void processInput() override;

        clove::vec2i getSize() const override;

        clove::Keyboard &getKeyboard() override;
        clove::Mouse &getMouse() override;
    };
}