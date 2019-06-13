#pragma once

#include "Clove/Platform/Window.hpp"

namespace clv{
	struct LinuxData{
		Display* display = nullptr;
		::Window* window;

		XVisualInfo** visual = nullptr;
	};

    class LinuxWindow : public Window{
        //VARIABLES
	private:
		Display* display = nullptr;
		Screen* screen = nullptr;
		int screenID;
        ::Window window;

        XEvent xevent;

		XVisualInfo* visual = nullptr; //TODO: unique_ptr with custom deleter
		XSetWindowAttributes windowAttribs;

		Atom atomWmDeleteWindow;

		LinuxData data;

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

		virtual void* getNativeWindow() const override;

		virtual void setVSync(bool enabled) override;
		virtual bool isVSync() const override;

	protected:
		virtual void processInput() override;

	private:
		void initialiseWindow(const WindowProps& props, gfx::API api);
    };
}