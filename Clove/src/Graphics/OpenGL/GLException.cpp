#include "Clove/Graphics/OpenGL/GLException.hpp"

#include <glad/glad.h>

namespace clv::gfx::ogl{
	void CLV_APIENTRY errorCallback(uint32 source, uint32 type, uint32 id, uint32 severity, int32 length, const char* msg, const void* data){
		switch(severity){
			case GL_DEBUG_SEVERITY_HIGH:
				CLV_LOG_ERROR(msg);
				break;

			case GL_DEBUG_SEVERITY_MEDIUM:
			case GL_DEBUG_SEVERITY_LOW:
				CLV_LOG_WARN(msg);
				break;

			case GL_DEBUG_SEVERITY_NOTIFICATION:
				CLV_LOG_TRACE(msg);
				break;
		}
	}
}