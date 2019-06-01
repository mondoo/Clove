#include "clvpch.hpp"
#include "Window.hpp"

#include "Clove/Graphics/Renderer.hpp"

namespace clv{
	Window::Window() = default;

	Window::~Window() = default;

	void Window::setEventCallbackFunction(const EventCallbackFn& callback){
		eventCallback = callback;
	}

	unsigned int Window::getWidth() const{
		return data.width;
	}

	unsigned int Window::getHeight() const{
		return data.height;
	}

	gfx::Renderer& Window::getRenderer(){
		return *renderer;
	}
}