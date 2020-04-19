#include "Clove/Platform/Window.hpp"

#include "Clove/Graphics/GraphicsFactory.hpp"
#include "Clove/Graphics/Surface.hpp"

namespace clv::plt{
	Window::Window() = default;

	Window::~Window() = default;

	void Window::beginFrame(){
		processInput();
	}

	void Window::endFrame(){
		surface->present();
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

	const std::shared_ptr<gfx::GraphicsFactory>& Window::getGraphicsFactory() {
		return graphicsFactory;
	}

	const std::shared_ptr<gfx::Surface>& Window::getSurface() const{
		return surface;
	}
}