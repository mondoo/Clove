#pragma once

#include "Clove/Platform/Window.hpp"

namespace clv{
    class LinuxWindow : public Window{
        //VARIABLES
	private:
		Display* display;
		int screenID;
        ::Window windowID;
        XEvent xevent;
		EventCallbackFn eventCallback;

        //FUNCTIONS
    public:
        LinuxWindow() = delete;
		LinuxWindow(const LinuxWindow& other) = delete;
		LinuxWindow(LinuxWindow&& other) noexcept = delete;
		LinuxWindow& operator=(const LinuxWindow& other) = delete;
		LinuxWindow& operator=(LinuxWindow&& other) noexcept = delete;
		virtual ~LinuxWindow();
		
		LinuxWindow(const WindowProps& props);
		
		virtual void beginFrame() override;
		virtual void endFrame() override;

		virtual void setEventCallbackFunction(const EventCallbackFn& callback) override;
		
		virtual void* getNativeWindow() const override;

		virtual unsigned int getWidth() const override;
		virtual unsigned int getHeight() const override;

		virtual void setVSync(bool enabled) override;
		virtual bool isVSync() const override;
    };
}