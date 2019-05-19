#include "clvpch.hpp"
#include "GLException.hpp"

#include <glad/glad.h>

namespace clv{
	namespace gfx{
		void GLClearError(){
			while(glGetError() != GL_NO_ERROR);
		}

		bool GLLogCall(){
			while(GLenum error = glGetError()){
				CLV_LOG_ERROR("OpenGL Error! ({0})", error);
				return false;
			}
			return true;
		}

		/*void APIENTRY errorCallback(unsigned int source, unsigned int type, unsigned int id, unsigned int severity, int length, const char * message, void * userParam){
			CLV_TRACE("IT WAS CALLED!");
		}*/
	}
}