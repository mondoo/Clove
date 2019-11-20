#pragma once

#include "Core/Platform/Window.hpp"

namespace clv::plt{
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
		int32 screenID;
        ::Window window;

        XEvent xevent;

		XVisualInfo* visual = nullptr; //TODO: unique_ptr with custom deleter
		XSetWindowAttributes windowAttribs;

		Atom atomWmDeleteWindow;

		LinuxData data;

        //FUNCTIONS
    public:
        LinuxWindow() = delete;
		LinuxWindow(const WindowProps& props);

		LinuxWindow(const LinuxWindow& other) = delete;
		LinuxWindow(LinuxWindow&& other) noexcept = delete;

		LinuxWindow& operator=(const LinuxWindow& other) = delete;
		LinuxWindow& operator=(LinuxWindow&& other) noexcept = delete;

		virtual ~LinuxWindow();

		virtual void* getNativeWindow() const override;

	protected:
		virtual void processInput() override;

	private:
		void initialiseWindow(const WindowProps& props);
    };
}