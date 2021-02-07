#pragma once

#include "Clove/Platform/Linux/CloveLinux.hpp"
#include "Clove/Platform/Window.hpp"

namespace garlic::clove {
    class GhaFactory;
}

namespace garlic::clove {
    class LinuxWindow : public Window {
        //VARIABLES
    private:
        bool open{ false };

        Display *display{ nullptr };
        ::Window window;
        Screen *screen{ nullptr };
        int32_t screenID;

        XEvent xevent{};

        XSetWindowAttributes windowAttribs{};

        Atom atomWmDeleteWindow;

        vec2i prevConfigureNotifySize{};

        Keyboard::Dispatcher keyboardDispatcher{};
        Mouse::Dispatcher mouseDispatcher{};

        //FUNCTIONS
    public:
        LinuxWindow() = delete;
        LinuxWindow(WindowDescriptor const &descriptor);

        LinuxWindow(LinuxWindow const &other)     = delete;
        LinuxWindow(LinuxWindow &&other) noexcept = delete;

        LinuxWindow &operator=(LinuxWindow const &other) = delete;
        LinuxWindow &operator=(LinuxWindow &&other) noexcept = delete;

        ~LinuxWindow();

        std::any getNativeWindow() const override;

        vec2i getPosition() const override;
        vec2i getSize() const override;

        void moveWindow(vec2i const &position) override;
        void resizeWindow(vec2i const &size) override;

        bool isOpen() const override;

        void close() override;

    protected:
        void processInput() override;
    };
}