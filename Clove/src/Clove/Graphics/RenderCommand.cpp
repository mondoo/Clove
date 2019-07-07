#include "clvpch.hpp"
#include "RenderCommand.hpp"

#include "Clove/Graphics/RenderAPI.hpp"

namespace clv::gfx{
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
		renderAPI = RenderAPI::initialiseRenderAPI(context);
	}
}