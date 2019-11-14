#import "Platform/Mac/CloveMac.h"
#include "CGLSurface.hpp"

#include "Core/Graphics/GraphicsTypes.hpp"

#include <OpenGL/CGLContext.h>

namespace clv::gfx::ogl{
	CGLSurface::CGLSurface(void* windowData){
		CLV_LOG_WARN("Warning! Clove with OpenGL on macOS has been halted. This is due to the fact that macOS only supports up to OpenGL 4.1. Clove currently does not work on macOS");
		
		CGLPixelFormatAttribute formatAttribs[] = {
			kCGLPFAOpenGLProfile, (CGLPixelFormatAttribute)kCGLOGLPVersion_GL4_Core,
			kCGLPFADoubleBuffer,
			(CGLPixelFormatAttribute)0
		};
		
		int numScreens;
		CGLChoosePixelFormat(formatAttribs, &pixelFormatObject, &numScreens);
		
		CGLError error = CGLCreateContext(pixelFormatObject, nullptr, &contextObject);
		if(error != kCGLNoError){
			CLV_DEBUG_BREAK; //TODO: Better error handling
		}
	}

	CGLSurface::CGLSurface(CGLSurface&& other) noexcept = default;
	
	CGLSurface& CGLSurface::operator=(CGLSurface&& other) noexcept = default;
	
	CGLSurface::~CGLSurface(){
		CGLDestroyContext(contextObject);
		CGLDestroyPixelFormat(pixelFormatObject);
	}
	
	void CGLSurface::makeCurrent(){
		CGLSetCurrentContext(contextObject);
	}

	void CGLSurface::setVSync(bool vsync){
		vsyncInt = vsync ? 1 : 0;
		CGLSetParameter(contextObject, kCGLCPSwapInterval, &vsyncInt);
	}
	
	bool CGLSurface::isVsync() const{
		return vsyncInt == 1;
	}

	void CGLSurface::present(){
		CGLFlushDrawable(contextObject);
	}
}
