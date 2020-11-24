#pragma once

#include "Clove/Platform/Linux/CloveLinux.hpp"
#include "Clove/Platform/Window.hpp"

namespace garlic::clove {
    class GraphicsFactory;
}

namespace clv::plt {
    class LinuxWindow : public Window {
        //VARIABLES
    private:
        bool open{ false };

        Display *display{ nullptr };
        ::Window window;
        Screen *screen{ nullptr };
        int32_t screenID;

        XEvent xevent;
		
        XSetWindowAttributes windowAttribs;

        Atom atomWmDeleteWindow;

        mth::vec2i prevConfigureNotifySize = {};

        //FUNCTIONS
    public:
        LinuxWindow() = delete;
        LinuxWindow(WindowDescriptor const &descriptor);
        LinuxWindow(Window const &parentWindow, mth::vec2i const &position, mth::vec2i const &size);

        LinuxWindow(LinuxWindow const &other)     = delete;
        LinuxWindow(LinuxWindow &&other) noexcept = delete;

        LinuxWindow &operator=(LinuxWindow const &other) = delete;
        LinuxWindow &operator=(LinuxWindow &&other) noexcept = delete;

        ~LinuxWindow();

        std::any getNativeWindow() const override;

        mth::vec2i getPosition() const override;
        mth::vec2i getSize() const override;

        void moveWindow(mth::vec2i const &position) override;
        void resizeWindow(mth::vec2i const &size) override;

        bool isOpen() const override;

        void close() override;

    protected:
        void processInput() override;
    };
}