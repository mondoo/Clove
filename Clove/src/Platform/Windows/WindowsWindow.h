#pragma once

#include "Clove/Window.h"

#include <GLFW/glfw3.h>

namespace clv{
	class WindowsWindow : public Window{
		//VARIABLES
	private:
		GLFWwindow* window;

		//For GLFW
		struct WindowData{
			std::string title;
			unsigned int width;
			unsigned int height;
			bool vSync;

			EventCallbackFn eventCallback;
		} data;

		//FUNCTIONS
	public:
		WindowsWindow(const WindowProps& props);
		virtual ~WindowsWindow();

		virtual void onUpdate() override;

		virtual inline unsigned int getWidth() const override{ return data.width; }
		virtual inline unsigned int getHeight() const override{ return data.height; }

		//Window attributes
		virtual inline void setEventCallback(const EventCallbackFn& callback) override{ data.eventCallback = callback; }
		virtual void setVSync(bool enabled) override;
		virtual bool isVSync() const override;

	private:
		virtual void init(const WindowProps& props);
		virtual void shutdown();
	};
}