#pragma once

#include "Clove/Platform/Window.hpp"
#include "Clove/Platform/Windows/CloveWindows.hpp"
#include "Clove/Platform/Windows/WindowsException.hpp"

namespace clv::plt {
    class WindowsWindow : public Window {
        //VARIABLES
    private:
        bool open = false;

        static LPCSTR constexpr className = "Clove";

        HINSTANCE instance;
        HWND windowsHandle;

        //FUNCTIONS
    public:
        WindowsWindow() = delete;
        WindowsWindow(WindowDescriptor const &descriptor);
        WindowsWindow(Window const &parentWindow, mth::vec2i const &position, mth::vec2i const &size);

        WindowsWindow(WindowsWindow const &other)     = delete;
        WindowsWindow(WindowsWindow &&other) noexcept = delete;

        WindowsWindow &operator=(WindowsWindow const &other) = delete;
        WindowsWindow &operator=(WindowsWindow &&other) noexcept = delete;

        ~WindowsWindow();

        void *getNativeWindow() const override;

        mth::vec2i getPosition() const override;
        mth::vec2i getSize() const override;

        void moveWindow(mth::vec2i const &position) override;
        void resizeWindow(mth::vec2i const &size) override;

        bool isOpen() const override;

        void close() override;

    protected:
        void processInput() override;

    private:
        static LRESULT CALLBACK HandleMsgSetup(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
        static LRESULT CALLBACK HandleMsgThunk(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
        LRESULT HandleMsg(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
    };
}