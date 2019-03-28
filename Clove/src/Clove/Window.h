#pragma once

#include "Clove/Events/Event.h"

namespace clv{
	class Renderer;

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
	class CLV_API Window{
	protected:
		using EventCallbackFn = std::function<void(Event&)>;
		
		//FUNCTIONS
	public:
		Window() = default;
		virtual ~Window() = default;

		virtual void onUpdate() = 0;

		virtual unsigned int getWidth() const = 0;
		virtual unsigned int getHeight() const = 0;

		//Window attributes
		virtual void setEventCallbackFunction(const EventCallbackFn& callback) = 0;
		virtual void setVSync(bool enabled) = 0;
		virtual bool isVSync() const = 0;

		virtual void* getNativeWindow() const = 0;

		virtual const Renderer getRenderer() const = 0;

		static Window* create(const WindowProps& props = WindowProps());
	};
}