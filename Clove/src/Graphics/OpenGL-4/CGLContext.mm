#import "Platform/Mac/CloveMac.h"
#include "CGLContext.hpp"

#include "Clove/Graphics/GraphicsTypes.hpp"

#include <OpenGL/CGLContext.h>

namespace clv::gfx{
	CGLContext::CGLContext(CGLContext&& other) noexcept = default;
	
	CGLContext& CGLContext::operator=(CGLContext&& other) noexcept = default;
	
	CGLContext::~CGLContext(){
		CGLDestroyContext(contextObject);
		CGLDestroyPixelFormat(pixelFormatObject);
	}
	
	CGLContext::CGLContext(void* windowData){
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
	
	void CGLContext::makeCurrent(){
		CGLSetCurrentContext(contextObject);
	}

	void CGLContext::setVSync(bool vsync){
		vsyncInt = vsync ? 1 : 0;
		CGLSetParameter(contextObject, kCGLCPSwapInterval, &vsyncInt);
	}
	
	bool CGLContext::isVsync() const{
		return vsyncInt == 1;
	}

	API CGLContext::getAPI() const{
		return API::OpenGL4;
	}

	void CGLContext::present(){
		CGLFlushDrawable(contextObject);
	}
}
