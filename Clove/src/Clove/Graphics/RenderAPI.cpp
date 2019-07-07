#include "clvpch.hpp"
#include "RenderAPI.hpp"

#include "Graphics/OpenGL-4/GL4RenderAPI.hpp"
#if CLV_PLATFORM_WINDOWS
#include "Graphics/DirectX-11/DX11RenderAPI.hpp"
#endif
#include "Clove/Graphics/Context.hpp"

namespace clv::gfx{
	std::unique_ptr<RenderAPI> RenderAPI::initialiseRenderAPI(const Context& context){
		switch(context.getAPI()){
			case API::OpenGL4:
				CLV_LOG_TRACE("Creating OpenGL renderer");
				return std::make_unique<GL4RenderAPI>(context);

			#if CLV_PLATFORM_WINDOWS
			case API::DirectX11:
				CLV_LOG_TRACE("Creating DirectX11 renderer");
				return std::make_unique<DX11RenderAPI>(context);
			#endif

			default:
				CLV_LOG_ERROR("Default statement hit. Could not initialise RenderAPI: {0}", __func__);
				break;
		}
	}
}