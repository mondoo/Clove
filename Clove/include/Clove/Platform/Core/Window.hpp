#pragma once

#include "Clove/Platform/Core/PlatformTypes.hpp"
#include "Clove/Core/Input/Keyboard.hpp"
#include "Clove/Core/Input/Mouse.hpp"

namespace clv::gfx{
	enum class API;
	class GraphicsFactory;
	class Surface;
}

namespace clv::plt{
	class Window{
		//VARIABLES
	public:
		utl::SingleCastDelegate<void()> onWindowCloseDelegate;
		utl::MultiCastDelegate<void(const mth::vec2ui&)> onWindowResize;

	protected:
		Keyboard keyboard;
		Mouse mouse;

		std::unique_ptr<gfx::GraphicsFactory> graphicsFactory;
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
		
		void setVSync(bool enabled);
		bool isVSync() const;

		virtual void* getNativeWindow() const = 0;

		virtual mth::vec2i getPosition() const = 0;
		virtual mth::vec2i getSize() const = 0;

		virtual void moveWindow(const mth::vec2i& position) = 0;
		virtual void resizeWindow(const mth::vec2i& size) = 0;

		Keyboard& getKeyboard();
		Mouse& getMouse();

		gfx::GraphicsFactory& getGraphicsFactory();
		const std::shared_ptr<gfx::Surface>& getSurface() const;

	protected:
		virtual void processInput() = 0;
	};
}