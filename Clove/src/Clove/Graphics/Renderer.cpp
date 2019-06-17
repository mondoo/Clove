#include "clvpch.hpp"
#include "Renderer.hpp"

#include "Graphics/OpenGL-4/GL4Renderer.hpp"
#if CLV_PLATFORM_WINDOWS
#include "Graphics/DirectX-11/DX11Renderer.hpp"
#endif
#include "Clove/Graphics/Context.hpp"

namespace clv::gfx{
	Renderer::Renderer() = default;

	Renderer::~Renderer() = default;

	std::unique_ptr<Renderer> Renderer::createRenderer(const Context& context){
		std::unique_ptr<Renderer> renderer;
		
		switch(context.getAPI()){
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
				CLV_LOG_ERROR("Default statement hit. No renderer initialised: {0}", __func__);
				break;
		}

		if(renderer == nullptr){
			CLV_ASSERT(false, "Renderer failed to initialise!");
		} else{
			CLV_LOG_DEBUG("Renderer created succesfully");
		}

		return renderer;
	}
}