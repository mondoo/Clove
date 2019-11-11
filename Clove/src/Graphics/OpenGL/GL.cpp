#include "GL.hpp"

#include "Graphics/OpenGL/GLRenderDevice.hpp"
#include "Graphics/OpenGL/GLRenderFactory.hpp"

namespace clv::gfx::ogl{
	std::pair<std::unique_ptr<RenderDevice>, std::unique_ptr<RenderFactory>> initialiseOGL(){
		auto glDevce = std::make_unique<GLRenderDevice>();
		auto glFactory = std::make_unique<GLRenderFactory>();

		return std::make_pair(std::move(glDevce), std::move(glFactory));
	}
}