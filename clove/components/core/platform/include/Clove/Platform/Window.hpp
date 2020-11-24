#pragma once

#include "Clove/Platform/Input/Keyboard.hpp"
#include "Clove/Platform/Input/Mouse.hpp"
#include "Clove/Platform/PlatformTypes.hpp"

#include <Clove/Delegate/MultiCastDelegate.hpp>
#include <Clove/Delegate/SingleCastDelegate.hpp>
#include <Clove/Maths/Vector.hpp>
#include <any>

namespace garlic::clove {
    enum class GraphicsApi;
    class GraphicsFactory;
    class Surface;
}

namespace garlic::clove {
    class Window {
        //VARIABLES
    public:
        SingleCastDelegate<void()> onWindowCloseDelegate;
        MultiCastDelegate<void(vec2ui const &)> onWindowResize;

    protected:
        Keyboard keyboard;
        Mouse mouse;

        //FUNCTIONS
    public:
        Window();

        Window(Window const &other)     = delete;
        Window(Window &&other) noexcept = delete;

        Window &operator=(Window const &other) = delete;
        Window &operator=(Window &&other) noexcept = delete;

        virtual ~Window();

        virtual void processInput() = 0;

        void setVSync(bool enabled);
        bool isVSync() const;

        virtual std::any getNativeWindow() const = 0;

        virtual vec2i getPosition() const = 0;
        virtual vec2i getSize() const     = 0;

        virtual void moveWindow(vec2i const &position) = 0;
        virtual void resizeWindow(vec2i const &size)   = 0;

        virtual bool isOpen() const = 0;

        virtual void close() = 0;

        Keyboard &getKeyboard();
        Mouse &getMouse();
    };
}