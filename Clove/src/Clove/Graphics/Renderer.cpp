#include "clvpch.hpp"
#include "Renderer.hpp"

#include "Graphics/OpenGL-4/GL4Renderer.hpp"
#include "Graphics/DirectX-11/DX11Renderer.hpp"

namespace clv::gfx{
	Renderer::Renderer() = default;

	Renderer::~Renderer() = default;

	std::unique_ptr<Renderer> Renderer::createRenderer(const Window& window, API api){
		std::unique_ptr<Renderer> renderer;
		
		switch(api){
			case API::OpenGL4:
				CLV_LOG_TRACE("Creating OpenGL renderer");
				renderer = std::make_unique<GL4Renderer>(window);
				break;

			case API::DirectX11:
				CLV_LOG_TRACE("Creating DirectX11 renderer");
				renderer = std::make_unique<DX11Renderer>(window);
				break;

			default:
				CLV_LOG_ERROR(__FUNCTION__ ": Default statement hit. No renderer initialised");
				break;
		}

		if(renderer == nullptr){
			CLV_ASSERT(false, "Renderer failed to initialise!");
		} else{
			CLV_LOG_INFO("Renderer created succesfully");
		}

		return renderer;
	}
}