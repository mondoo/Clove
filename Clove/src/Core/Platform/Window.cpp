#include "Window.hpp"

#include "Core/Graphics/Renderer.hpp"
#include "Core/Graphics/Surface.hpp"

namespace clv::plt{
	Window::Window() = default;

	Window::~Window() = default;

	void Window::beginFrame(){
		processInput();
	}

	void Window::endFrame(){
		surface->present();
	}

	uint32 Window::getWidth() const{
		return windowProperties.width;
	}

	uint32 Window::getHeight() const{
		return windowProperties.height;
	}

	void Window::setVSync(bool enabled){
		surface->setVSync(enabled);
	}

	bool Window::isVSync() const{
		return surface->isVsync();
	}

	Keyboard& Window::getKeyboard(){
		return keyboard;
	}

	Mouse& Window::getMouse(){
		return mouse;
	}

	gfx::Surface& Window::getSurface(){
		return *surface.get();
	}
}