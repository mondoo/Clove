#pragma once

#include "Clove/Input/Keyboard.hpp"
#include "Clove/Input/Mouse.hpp"

namespace clv{
	class Renderer;
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

		//FUNCTIONS
	public:
		Window() = default;
		Window(const Window& other) = delete;
		Window(Window&& other) noexcept = delete;

		virtual ~Window() = default;

		virtual void beginFrame() = 0;

		virtual void setEventCallbackFunction(const EventCallbackFn& callback) = 0;
		
		CLV_API virtual unsigned int getWidth() const = 0;
		CLV_API virtual unsigned int getHeight() const = 0;

		CLV_API virtual void setVSync(bool enabled) = 0;
		CLV_API virtual bool isVSync() const = 0;

		CLV_API inline Keyboard& getKeyboard();
		CLV_API inline Mouse& getMouse();

		static Window* create(const WindowProps& props = WindowProps());

		Window& operator=(const Window& other) = delete;
		Window& operator=(Window&& other) noexcept = delete;
	};
}

#include "Window.inl"