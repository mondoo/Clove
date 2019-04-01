#pragma once

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
	protected:
		using EventCallbackFn = std::function<void(Event&)>;
		
		//FUNCTIONS
	public:
		CLV_API Window() = default;
		CLV_API virtual ~Window() = default;

		CLV_API virtual void onUpdate() = 0;

		CLV_API virtual unsigned int getWidth() const = 0;
		CLV_API virtual unsigned int getHeight() const = 0;

		//Window attributes
		CLV_API virtual void setEventCallbackFunction(const EventCallbackFn& callback) = 0;
		CLV_API virtual void setVSync(bool enabled) = 0;
		CLV_API virtual bool isVSync() const = 0;

		CLV_API virtual void* getNativeWindow() const = 0;

		CLV_API virtual const Renderer& getRenderer() const = 0;

		CLV_API static Window* create(const WindowProps& props = WindowProps());
	};
}