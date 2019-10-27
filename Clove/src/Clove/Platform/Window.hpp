#pragma once

#include "Clove/Platform/PlatformTypes.hpp"
#include "Clove/Input/Keyboard.hpp"
#include "Clove/Input/Mouse.hpp"
#include "Clove/Utils/Delegate.hpp"

namespace clv::gfx{
	enum class API;
	class Context;
}

namespace clv::plt{
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

		uint32 getWidth() const;
		uint32 getHeight() const;

		void setVSync(bool enabled);
		bool isVSync() const;

		Keyboard& getKeyboard();
		Mouse& getMouse();

		gfx::Context& getContext();

	protected:
		virtual void processInput() = 0;
	};
}