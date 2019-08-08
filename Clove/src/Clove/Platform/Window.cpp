#include "clvpch.hpp"
#include "Window.hpp"

#include "Clove/Graphics/Renderer.hpp"
#include "Clove/Graphics/Context.hpp"

namespace clv{
	Window::Window() = default;

	Window::~Window() = default;

	void Window::beginFrame(){
		processInput();
	}

	void Window::endFrame(){
		context->present();
	}

	uint32 Window::getWidth() const{
		return windowProperties.width;
	}

	uint32 Window::getHeight() const{
		return windowProperties.height;
	}

	void Window::setVSync(bool enabled){
		context->setVSync(enabled);
	}

	bool Window::isVSync() const{
		return context->isVsync();
	}

	Keyboard& Window::getKeyboard(){
		return keyboard;
	}

	Mouse& Window::getMouse(){
		return mouse;
	}

	gfx::Context& Window::getContext(){
		return *context.get();
	}
}