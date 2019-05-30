#include "clvpch.hpp"
#include "GLException.hpp"

#include <glad/glad.h>

namespace clv::gfx{
	void APIENTRY errorCallback(unsigned int source, unsigned int type, unsigned int id,
								unsigned int severity, int length,
								const char *msg, const void *data){
		std::string format = "GL Debug message:\n| Source: {0} | Type: {1} | Severity: {2} |\nMessage {3}";

		std::string mesage = msg;

		std::string msgSource;
		std::string msgType;

		switch(source){
			case GL_DEBUG_SOURCE_API:
				msgSource = "API";
				break;

			case GL_DEBUG_SOURCE_WINDOW_SYSTEM:
				msgSource = "WINDOW SYSTEM";
				break;

			case GL_DEBUG_SOURCE_SHADER_COMPILER:
				msgSource = "SHADER COMPILER";
				break;

			case GL_DEBUG_SOURCE_THIRD_PARTY:
				msgSource = "THIRD PARTY";
				break;

			case GL_DEBUG_SOURCE_APPLICATION:
				msgSource = "APPLICATION";
				break;

			case GL_DEBUG_SOURCE_OTHER:
				msgSource = "UNKNOWN";
				break;

			default:
				msgSource = "UNKNOWN";
				break;
		}

		switch(type){
			case GL_DEBUG_TYPE_ERROR:
				msgType = "ERROR";
				break;

			case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR:
				msgType = "DEPRECATED BEHAVIOR";
				break;

			case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR:
				msgType = "UDEFINED BEHAVIOR";
				break;

			case GL_DEBUG_TYPE_PORTABILITY:
				msgType = "PORTABILITY";
				break;

			case GL_DEBUG_TYPE_PERFORMANCE:
				msgType = "PERFORMANCE";
				break;

			case GL_DEBUG_TYPE_OTHER:
				msgType = "OTHER";
				break;

			case GL_DEBUG_TYPE_MARKER:
				msgType = "MARKER";
				break;

			default:
				msgType = "UNKNOWN";
				break;
		}

		switch(severity){
			case GL_DEBUG_SEVERITY_HIGH:
				CLV_LOG_ERROR(format.c_str(), msgSource, msgType, "HIGH", mesage);
				break;

			case GL_DEBUG_SEVERITY_MEDIUM:
				CLV_LOG_WARN(format.c_str(), msgSource, msgType, "MEDIUM", mesage);
				break;

			case GL_DEBUG_SEVERITY_LOW:
				CLV_LOG_WARN(format.c_str(), msgSource, msgType, "LOW", mesage);
				break;

			case GL_DEBUG_SEVERITY_NOTIFICATION:
				CLV_LOG_TRACE(format.c_str(), msgSource, msgType, "NOTIFICATION", mesage);
				break;
		}
	}
}