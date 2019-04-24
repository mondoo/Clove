#include "clvpch.hpp"
#include "Clove/Rendering/API/GLHelpers.hpp"

#include <glad/glad.h>

namespace clv{
	void GLClearError(){
		while(glGetError() != GL_NO_ERROR);
	}

	bool GLLogCall(){
		while(GLenum error = glGetError()){
			CLV_ERROR("OpenGL Error! ({0})", error);
			return false;
		}
		return true;
	}
}