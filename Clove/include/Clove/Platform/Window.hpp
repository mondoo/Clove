#pragma once

#include "Clove/Input/Keyboard.hpp"
#include "Clove/Input/Mouse.hpp"
#include "Clove/Platform/PlatformTypes.hpp"

namespace clv::gfx {
    enum class API;
    class GraphicsFactory;
    class Surface;
}

namespace clv::plt {
    class Window {
        //VARIABLES
    public:
        garlic::SingleCastDelegate<void()> onWindowCloseDelegate;
        garlic::MultiCastDelegate<void(const mth::vec2ui&)> onWindowResize;

    protected:
        Keyboard keyboard;
        Mouse mouse;

        std::shared_ptr<gfx::GraphicsFactory> graphicsFactory;
        std::shared_ptr<gfx::Surface> surface;

        //FUNCTIONS
    public:
        Window();

        Window(const Window& other)     = delete;
        Window(Window&& other) noexcept = delete;

        Window& operator=(const Window& other) = delete;
        Window& operator=(Window&& other) noexcept = delete;

        virtual ~Window();

        void beginFrame();
        void endFrame();

        void setVSync(bool enabled);
        bool isVSync() const;

        virtual void* getNativeWindow() const = 0;

        virtual mth::vec2i getPosition() const = 0;
        virtual mth::vec2i getSize() const     = 0;

        virtual void moveWindow(const mth::vec2i& position) = 0;
        virtual void resizeWindow(const mth::vec2i& size)   = 0;

        virtual bool isOpen() const = 0;

        virtual void close() = 0;

        Keyboard& getKeyboard();
        Mouse& getMouse();

        const std::shared_ptr<gfx::GraphicsFactory>& getGraphicsFactory();
        const std::shared_ptr<gfx::Surface>& getSurface() const;

    protected:
        virtual void processInput() = 0;
    };
}