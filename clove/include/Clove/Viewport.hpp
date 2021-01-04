#pragma once

namespace garlic::clove {
    class Keyboard;
    class Mouse;
}

namespace garlic::clove {
    /**
     * @brief Provides an abstraction over a renderable area that handles input.
     */
    class Viewport {
        //FUNCTIONS
    public:
        virtual ~Viewport() = default;

        virtual void processInput() = 0;

        virtual Keyboard &getKeyboard() = 0;
        virtual Mouse &getMouse()       = 0;
    };
}