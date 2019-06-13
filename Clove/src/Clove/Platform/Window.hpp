#pragma once

#include "Clove/Input/Keyboard.hpp"
#include "Clove/Input/Mouse.hpp"

namespace clv::gfx{
	enum class API;
	class Context;
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
		EventCallbackFn eventCallback;

		WindowProps windowProperties;

		bool vSync = true;

		Keyboard keyboard;
		Mouse mouse;
		std::unique_ptr<gfx::Context> context;

		//FUNCTIONS
	public:
		Window();
		Window(const Window& other) = delete;
		Window(Window&& other) noexcept = delete;
		Window& operator=(const Window& other) = delete;
		Window& operator=(Window&& other) noexcept = delete;
		virtual ~Window();

		void beginFrame();
		void endFrame();

		void setEventCallbackFunction(const EventCallbackFn& callback);
		
		virtual void* getNativeWindow() const = 0;

		unsigned int getWidth() const;
		unsigned int getHeight() const;

		virtual void setVSync(bool enabled) = 0;
		virtual bool isVSync() const = 0;

		Keyboard& getKeyboard();
		Mouse& getMouse();

		gfx::Context& getContext();

		//Defined in derived class
		static Window* create(const WindowProps& props = WindowProps());
		static Window* create(const WindowProps& props, gfx::API api);
	protected:
		virtual void processInput() = 0;
	};
}