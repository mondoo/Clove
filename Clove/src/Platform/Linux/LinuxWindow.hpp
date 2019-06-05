#pragma once

#include "Clove/Platform/Window.hpp"

namespace clv{
    class LinuxWindow : public Window{
        //VARIABLES
	private:
		Display* display = nullptr;
		Screen* screen = nullptr;
		int screenID;
        ::Window window;

        XEvent xevent;

		XVisualInfo* visual = nullptr;
		XSetWindowAttributes windowAttribs;

		Atom atomWmDeleteWindow;
		
        //FUNCTIONS
    public:
        LinuxWindow() = delete;
		LinuxWindow(const LinuxWindow& other) = delete;
		LinuxWindow(LinuxWindow&& other) noexcept = delete;
		LinuxWindow& operator=(const LinuxWindow& other) = delete;
		LinuxWindow& operator=(LinuxWindow&& other) noexcept = delete;
		virtual ~LinuxWindow();
		
		LinuxWindow(const WindowProps& props);
		LinuxWindow(const WindowProps& props, gfx::API api);
		
		virtual void beginFrame() override;
		virtual void endFrame() override;

		virtual void* getNativeWindow() const override;

		virtual void setVSync(bool enabled) override;
		virtual bool isVSync() const override;

	private:
		void initialiseWindow(const WindowProps& props, gfx::API api);
    };
}