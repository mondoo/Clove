#include "clvpch.hpp"
#include "RenderCommand.hpp"

#include "Graphics/OpenGL-4/GL4RenderAPI.hpp"
#if CLV_PLATFORM_WINDOWS
#include "Graphics/DirectX-11/DX11RenderAPI.hpp"
#endif
#include "Clove/Graphics/Context.hpp"

namespace clv::gfx{
	std::unique_ptr<RenderAPI> RenderCommand::renderAPI;

	void RenderCommand::clear(){
		renderAPI->clear();
	}

	void RenderCommand::drawIndexed(const unsigned int count){
		renderAPI->drawIndexed(count);
	}

	void RenderCommand::setClearColour(const math::Vector4f& colour){
		renderAPI->setClearColour(colour);
	}

	void RenderCommand::initialiseRenderAPI(const Context& context){
		switch(context.getAPI()){
			case API::OpenGL4:
				CLV_LOG_TRACE("Creating OpenGL renderer");
				renderAPI = std::make_unique<GL4RenderAPI>(context);
				break;

			#if CLV_PLATFORM_WINDOWS
			case API::DirectX11:
				CLV_LOG_TRACE("Creating DirectX11 renderer");
				renderAPI = std::make_unique<DX11RenderAPI>(context);
				break;
			#endif

			default:
				CLV_LOG_ERROR("Default statement hit. Could not initialise RenderAPI: {0}", __func__);
				break;
		}
	}
}