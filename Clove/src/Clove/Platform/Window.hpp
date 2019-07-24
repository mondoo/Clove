#pragma once

#include "Clove/Input/Keyboard.hpp"
#include "Clove/Input/Mouse.hpp"
#include "Clove/Utils/Delegate.hpp"

namespace clv::gfx{
	enum class API;
	class Context;
}

namespace clv{
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

	class Window{
		//VARIABLES
	public:
		utl::SingleCastDelegate<void()> onWindowCloseDelegate;

	protected:
		WindowProps windowProperties;

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
		
		virtual void* getNativeWindow() const = 0;

		unsigned int getWidth() const;
		unsigned int getHeight() const;

		void setVSync(bool enabled);
		bool isVSync() const;

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