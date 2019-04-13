#pragma once

#include "Clove/Window.hpp"

#include "Clove/Rendering/Renderer.hpp"

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

		std::unique_ptr<Renderer> renderer;

		//FUNCTIONS
	public:
		WindowsWindow(const WindowProps& props);
		virtual ~WindowsWindow();

		virtual void beginFrame() override;
		virtual void endFrame() override;

		virtual inline unsigned int getWidth() const override;
		virtual inline unsigned int getHeight() const override;

		virtual inline void setEventCallbackFunction(const EventCallbackFn& callback) override;
		virtual void setVSync(bool enabled) override;
		virtual bool isVSync() const override;

		virtual void* getNativeWindow() const override;
		virtual Renderer& getRenderer() override;

	private:
		void init(const WindowProps& props);
		void shutdown();
	};
}