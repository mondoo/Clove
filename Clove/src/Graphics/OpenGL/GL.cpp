#include "Clove/Graphics/OpenGL/GL.hpp"

#include "Clove/Graphics/OpenGL/GLGraphicsFactory.hpp"
#include "Clove/Graphics/OpenGL/GLException.hpp"

#include <glad/glad.h>

namespace clv::gfx::ogl{
	std::shared_ptr<GraphicsFactory> initialiseOGL() {
		return std::make_shared<GLGraphicsFactory>();
	}
}