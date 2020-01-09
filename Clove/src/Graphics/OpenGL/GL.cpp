#include "Clove/Graphics/OpenGL/GL.hpp"

#include "Clove/Graphics/OpenGL/GLRenderFactory.hpp"
#include "Clove/Graphics/OpenGL/GLException.hpp"

#include <glad/glad.h>

namespace clv::gfx::ogl{
	std::unique_ptr<RenderFactory> initialiseOGL(){
		return std::make_unique<GLRenderFactory>();
	}
}