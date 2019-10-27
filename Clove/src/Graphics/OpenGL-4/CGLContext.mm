#include "CGLContext.hpp"

#include "Clove/Graphics/GraphicsTypes.hpp"

namespace clv::gfx{
	CGLContext::CGLContext(CGLContext&& other) noexcept = default;
	
	CGLContext& CGLContext::operator=(CGLContext&& other) noexcept = default;
	
	CGLContext::~CGLContext() = default;
	
	CGLContext::CGLContext(void* windowData){
		
	}
	
	void CGLContext::makeCurrent(){
	
	}

	void CGLContext::setVSync(bool vsync){
		
	}
	
	bool CGLContext::isVsync() const{
		
	}

	API CGLContext::getAPI() const{
		return API::OpenGL4;
	}

	void CGLContext::present(){
		
	}
}
