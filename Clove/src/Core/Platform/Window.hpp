#pragma once

#include "Core/Platform/PlatformTypes.hpp"
#include "Core/Input/Keyboard.hpp"
#include "Core/Input/Mouse.hpp"

namespace clv::gfx{
	enum class API;
	class Surface;
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

		std::shared_ptr<gfx::Surface> surface;

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

		gfx::Surface& getSurface();

	protected:
		virtual void processInput() = 0;
	};
}