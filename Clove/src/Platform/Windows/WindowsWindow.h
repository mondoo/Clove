#pragma once

#include "Clove/Window.h"
#include "Clove/Rendering/Renderer.h"

struct GLFWwindow;

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

		Renderer renderer;

		//FUNCTIONS
	public:
		WindowsWindow(const WindowProps& props);
		virtual ~WindowsWindow();

		virtual void onUpdate() override;

		virtual inline unsigned int getWidth() const override;
		virtual inline unsigned int getHeight() const override;

		//Window attributes
		virtual inline void setEventCallbackFunction(const EventCallbackFn& callback) override;
		virtual void setVSync(bool enabled) override;
		virtual bool isVSync() const override;

		virtual void* getNativeWindow() const override;
		virtual const Renderer& getRenderer() const override;

	private:
		virtual void init(const WindowProps& props);
		virtual void shutdown();
	};
}