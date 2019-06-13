#include "clvpch.hpp"
#include "Renderer.hpp"

#include "Graphics/OpenGL-4/GL4Renderer.hpp"
#if CLV_PLATFORM_WINDOWS
#include "Graphics/DirectX-11/DX11Renderer.hpp"
#endif

namespace clv::gfx{
	Renderer::Renderer() = default;

	Renderer::~Renderer() = default;

	std::unique_ptr<Renderer> Renderer::createRenderer(const Context& context, API api){
		std::unique_ptr<Renderer> renderer;
		
		switch(api){
			case API::OpenGL4:
				CLV_LOG_TRACE("Creating OpenGL renderer");
				renderer = std::make_unique<GL4Renderer>(context);
				break;
			#if CLV_PLATFORM_WINDOWS
			case API::DirectX11:
				CLV_LOG_TRACE("Creating DirectX11 renderer");
				renderer = std::make_unique<DX11Renderer>(context);
				break;
			#endif
			default:
				//CLV_LOG_ERROR(__FUNCTION__ ": Default statement hit. No renderer initialised");
				//TODO: figure out above for different compilers
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