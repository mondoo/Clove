#pragma once

#include "Clove/Viewport.hpp"

#include <memory>

namespace garlic::clove {
    class Window;
}

namespace garlic::clove {
    /**
     * @brief A Viewport backed by a platform Window.
     */
    class WindowViewport : public Viewport {
        //VARIABLES
    private:
        std::unique_ptr<Window> window{};

        //FUNCTIONS
    public:
        WindowViewport() = delete;
        WindowViewport(std::unique_ptr<Window> window);

        WindowViewport(WindowViewport const &other) = delete;
        WindowViewport(WindowViewport &&other) noexcept;

        WindowViewport &operator=(WindowViewport const &other) = delete;
        WindowViewport &operator=(WindowViewport &&other) noexcept;

        ~WindowViewport();

        void processInput() override;

        Keyboard &getKeyboard() override;
        Mouse &getMouse() override;
    };
}