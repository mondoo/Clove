#pragma once

#include "Clove/Window.hpp"

struct GLFWwindow;

namespace clv{
	class WindowsWindow : public Window{
		//VARIABLES
	private:
		GLFWwindow* window = nullptr;

		//For GLFW
		struct WindowData{
			std::string title = "";
			unsigned int width = 0;
			unsigned int height = 0;
			bool vSync = false;

			EventCallbackFn eventCallback;
		} data;

		//FUNCTIONS
	public:
		WindowsWindow(const WindowProps& props);
		virtual ~WindowsWindow();

		virtual void swapBuffers() override;

		virtual inline unsigned int getWidth() const override;
		virtual inline unsigned int getHeight() const override;

		virtual inline void setEventCallbackFunction(const EventCallbackFn& callback) override;

		virtual void setVSync(bool enabled) override;
		virtual bool isVSync() const override;

		virtual void* getNativeWindow() const override;

	private:
		void init(const WindowProps& props);
		void shutdown();
	};
}

#include "WindowsWindow.inl"