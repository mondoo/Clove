#pragma once

#include "Clove/Platform/Input/Keyboard.hpp"
#include "Clove/Platform/Input/Mouse.hpp"

#include <Clove/Delegate/MultiCastDelegate.hpp>
#include <Clove/Delegate/SingleCastDelegate.hpp>
#include <Clove/Maths/Vector.hpp>
#include <any>

namespace garlic::clove {
    /**
     * @brief An abstraction over a platform's native window
     */
    class Window {
        //TYPES
    public:
        struct Descriptor {
            std::string title{ "Clove window" };
            int32_t width{ 800 };
            int32_t height{ 600 };
        };

        //VARIABLES
    public:
        SingleCastDelegate<void()> onWindowCloseDelegate{};
        MultiCastDelegate<void(vec2ui const &)> onWindowResize{};

    protected:
        Keyboard keyboard;
        Mouse mouse;

        //FUNCTIONS
    public:
        Window() = delete;
        Window(Keyboard::Dispatcher &keyboardDispatcher, Mouse::Dispatcher &mouseDispatcher)
            : keyboard{ keyboardDispatcher }
            , mouse{ mouseDispatcher } {
        }

        Window(Window const &other)     = delete;
        Window(Window &&other) noexcept = delete;

        Window &operator=(Window const &other) = delete;
        Window &operator=(Window &&other) noexcept = delete;

        virtual ~Window() = default;

        static std::unique_ptr<Window> create(Descriptor descriptor);

        /**
         * @brief Pumps the message queue of the window. Populating the Mouse and Keyboard classes with events.
         */
        virtual void processInput() = 0;

        virtual std::any getNativeWindow() const = 0;

        /**
         * @brief Returns the poisition of the window in screen space coordinates.
         * @param clientArea If true, will return the client area of the window (inside borders / captionbar etc.).
         * @return A vec2i containing the screen space coordinates.
         */
        virtual vec2i getPosition(bool clientArea) const = 0;
        virtual vec2i getSize() const                    = 0;

        virtual void moveWindow(vec2i const &position) = 0;
        virtual void resizeWindow(vec2i const &size)   = 0;

        virtual bool isOpen() const = 0;

        virtual void close() = 0;

        inline Keyboard &getKeyboard();
        inline Mouse &getMouse();
    };
}

#include "Window.inl"