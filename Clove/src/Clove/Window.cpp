#include "clvpch.hpp"
#include "Window.hpp"

#include "Clove/Graphics/Renderer.hpp"

namespace clv{
	void Window::initialiseRenderer(graphics::API api){
		switch(api){
			case graphics::API::OpenGL:
				CLV_TRACE("Creating OpenGL renderer");
				renderer = std::unique_ptr<graphics::Renderer>(graphics::Renderer::createOpenGLRenderer(*this));
				break;

			case graphics::API::DX11:
				CLV_TRACE("Creating DirectX11 renderer");
				renderer = std::unique_ptr<graphics::Renderer>(graphics::Renderer::createDirectX11Renderer(*this));
				break;

			default:
				CLV_ERROR(__FUNCTION__ ": Default statement hit. No renderer initialised");
				break;
		}

		CLV_ASSERT(renderer != nullptr, "Renderer failed to initialise!");
		CLV_INFO("Renderer created succesfully");
	}
}