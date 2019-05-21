#pragma once

#include "Clove/Input/Keyboard.hpp"
#include "Clove/Input/Mouse.hpp"

namespace clv::gfx{
	class Renderer;
}

namespace clv{
	class Event;

	struct WindowProps{
		std::string title;
		unsigned int width;
		unsigned int height;

		WindowProps(const std::string& inTitle = "Clove Engine", unsigned int inWidth = 1280, unsigned int inHeight = 720)
			: title(inTitle)
			, width(inWidth)
			, height(inHeight){
		}
	};

	//Interface representing a desktop system based window
	class Window{
		//DEFINITIONS
	protected:
		using EventCallbackFn = std::function<void(Event&)>;
		
		//VARIABLES
	protected:
		Keyboard keyboard;
		Mouse mouse;
		std::unique_ptr<gfx::Renderer> renderer;

		//FUNCTIONS
	public:
		Window();
		Window(const Window& other) = delete;
		Window(Window&& other) noexcept = delete;
		Window& operator=(const Window& other) = delete;
		Window& operator=(Window&& other) noexcept = delete;
		virtual ~Window();

		virtual void beginFrame() = 0;
		virtual void endFrame() = 0;

		virtual void setEventCallbackFunction(const EventCallbackFn& callback) = 0;
		
		virtual void* getNativeWindow() const = 0;

		virtual unsigned int getWidth() const = 0;
		virtual unsigned int getHeight() const = 0;

		virtual void setVSync(bool enabled) = 0;
		virtual bool isVSync() const = 0;

		inline Keyboard& getKeyboard();
		inline Mouse& getMouse();
		gfx::Renderer& getRenderer();

		static Window* create(const WindowProps& props = WindowProps());
	};
}

#include "Window.inl"