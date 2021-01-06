#pragma once

#include "Clove/Surface.hpp"

#include <memory>

namespace garlic::clove {
    class Window;
}

namespace garlic::clove {
    /**
     * @brief A Surface backed by a platform Window.
     */
    class WindowSurface : public Surface {
        //VARIABLES
    private:
        std::unique_ptr<Window> window{};

        //FUNCTIONS
    public:
        WindowSurface() = delete;
        WindowSurface(std::unique_ptr<Window> window);

        WindowSurface(WindowSurface const &other) = delete;
        WindowSurface(WindowSurface &&other) noexcept;

        WindowSurface &operator=(WindowSurface const &other) = delete;
        WindowSurface &operator=(WindowSurface &&other) noexcept;

        ~WindowSurface();

        void processInput() override;

        Keyboard &getKeyboard() override;
        Mouse &getMouse() override;
    };
}