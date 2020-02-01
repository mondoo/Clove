#include "Clove/Graphics/OpenGL/GLException.hpp"

#include <glad/glad.h>

namespace clv::gfx::ogl{
	void CLV_APIENTRY errorCallback(uint32_t source, uint32_t type, uint32_t id, uint32_t severity, int32_t length, const char* msg, const void* data){
		switch(severity){
			case GL_DEBUG_SEVERITY_HIGH:
				CLV_LOG_ERROR(msg);
				break;

			case GL_DEBUG_SEVERITY_MEDIUM:
			case GL_DEBUG_SEVERITY_LOW:
				CLV_LOG_WARN(msg);
				break;
		}
	}
}