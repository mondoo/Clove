#include "clvpch.hpp"
#include "Window.hpp"

#include "Clove/Graphics/Renderer.hpp"

namespace clv{
	Window::Window() = default;

	Window::~Window() = default;

	gfx::Renderer& Window::getRenderer(){
		return *renderer;
	}
}