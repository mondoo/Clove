#include "clvpch.hpp"
#include "GLRenderer.hpp"

#include "Clove/Platform/Window.hpp"
#include "Graphics/OpenGL/GLException.hpp"
#include "Clove/Scene/Scene.hpp"
#include "Clove/Scene/CameraSceneNode.hpp"
#include "Clove/Scene/Lights/PointLightSceneNode.hpp"
#include "Clove/Scene/Lights/DirectionalLightSceneNode.hpp"

#if CLV_PLATFORM_WINDOWS
#include "Platform/Windows/WindowsException.hpp"
#endif

#include <glad/glad.h>
#include <gl/GL.h>
#include <wglext.h>

void APIENTRY errorCallback(GLenum source, GLenum type, GLuint id,
							GLenum severity, GLsizei length,
							const GLchar *msg, const void *data){
	std::string format = "GL Debug message:\n| Source: {0} | Type: {1} | Severity: {2} |\nMessage {3}";

	std::string mesage = msg;

	//TODO: Move to GLException
	//TODO: Remove the GLCall shinangins I guess

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

namespace clv::gfx{
	GLRenderer::~GLRenderer(){
	#if CLV_PLATFORM_WINDOWS
		ReleaseDC(windowsHandle, windowsDeviceContext);
		wglDeleteContext(windowsResourceContext);
	#endif
	}

	GLRenderer::GLRenderer(const Window& window){
	#if CLV_PLATFORM_WINDOWS
		windowsHandle = reinterpret_cast<HWND>(window.getNativeWindow());

		windowsDeviceContext = GetDC(windowsHandle);

		PIXELFORMATDESCRIPTOR pfd = { 0 };
		pfd.nSize = sizeof(pfd);
		pfd.nVersion = 1;
		pfd.dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;
		pfd.iPixelType = PFD_TYPE_RGBA;
		pfd.cColorBits = 32;
		pfd.cAlphaBits = 8;
		pfd.cDepthBits = 24;

		int pf;
		pf = ChoosePixelFormat(windowsDeviceContext, &pfd);
		if(pf == 0){
			throw CLV_WINDOWS_LAST_EXCEPTION;
		}

		if(SetPixelFormat(windowsDeviceContext, pf, &pfd) == FALSE){
			throw CLV_WINDOWS_LAST_EXCEPTION;
		}

		//Create a temp context to get the proecAdresss for wglCreateContextAttribsARB
		HGLRC tempOpenGLContext = wglCreateContext(windowsDeviceContext);
		wglMakeCurrent(windowsDeviceContext, tempOpenGLContext);

		PFNWGLCREATECONTEXTATTRIBSARBPROC wglCreateContextAttribsARB = (PFNWGLCREATECONTEXTATTRIBSARBPROC)wglGetProcAddress("wglCreateContextAttribsARB");
		if(wglCreateContextAttribsARB){
			int attributes[] = {
				WGL_CONTEXT_MAJOR_VERSION_ARB, 4,
				WGL_CONTEXT_MINOR_VERSION_ARB, 6,
				WGL_CONTEXT_FLAGS_ARB, WGL_CONTEXT_FORWARD_COMPATIBLE_BIT_ARB,
				0
			};

			//Create the new 4.6 context
			windowsResourceContext = wglCreateContextAttribsARB(windowsDeviceContext, NULL, attributes);

			//Remove the old temp one
			wglMakeCurrent(NULL, NULL);
			wglDeleteContext(tempOpenGLContext);

			//Swap in the fancy new one
			wglMakeCurrent(windowsDeviceContext, windowsResourceContext);

			CLV_LOG_INFO("Succesfully created an OpenGL 4.6 context");
		} else{
			CLV_LOG_WARN("Could not retrieve wglCreateContextAttribsARB. Application might not support OpenGL 3.2+ contexts");

			windowsResourceContext = wglCreateContext(windowsDeviceContext);
			wglMakeCurrent(windowsDeviceContext, windowsResourceContext);
		}

	#endif

		CLV_LOG_DEBUG("Device context created");

		CLV_ASSERT(gladLoadGL(), "Failed to load OpenGL functions");

		CLV_LOG_INFO("Rendering environment created successfully!");
		CLV_LOG_INFO("GL version: {0}", glGetString(GL_VERSION));

		CLV_LOG_TRACE("Enabling Depth buffer");
		GLCall(glDepthFunc(GL_LESS));
		GLCall(glEnable(GL_DEPTH_TEST));

		CLV_LOG_TRACE("Blend set to: SRC_ALPHA | ONE_MINUS_SRC_ALPHA");
		//src is from the image - dest is what is already in the buffer
		GLCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));
		GLCall(glEnable(GL_BLEND));
		//I guess it's called blending because you blend the src with the destination

		GLCall(glClearColor(0.0f, 0.0f, 1.0f, 1.0f));

		GLCall(glFrontFace(GL_CCW));

		GLCall(glDebugMessageCallback(errorCallback, nullptr));
		GLCall(glEnable(GL_DEBUG_OUTPUT));
		GLCall(glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS));
	}

	void GLRenderer::clear(){
	#if CLV_PLATFORM_WINDOWS
		SwapBuffers(windowsDeviceContext);
	#endif
		GLCall(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));
	}

	void GLRenderer::drawIndexed(const unsigned int count){
		GLCall(glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(count), GL_UNSIGNED_INT, nullptr));
	}
}