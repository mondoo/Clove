#include "Clove/Platform/Mac/CloveMac.h"
#include "Clove/Graphics/Metal/MTLSurface.hpp"

#include "Clove/Platform/Mac/MacWindow.hpp"

@implementation MTLSurfaceProxy
- (instancetype)initWithDevice:(id<MTLDevice>)mtlDevice windowData: (void*)windowData{
	clv::plt::MacData* data = reinterpret_cast<clv::plt::MacData*>(windowData);
	
	const NSRect rect = NSMakeRect(0, 0, data->size.x, data->size.y);
	_view = [[[MTKView alloc] initWithFrame:rect] autorelease];
	
	//[_view setDelegate:self];
	[_view setDevice:mtlDevice];
	
	//[_view setClearColor:MTLClearColorMake(0.0, 0.4, 0.21, 1.0)]; //TODO: Put this in with the thing
	
	return self;
}

@end

namespace clv::gfx::mtl{
	MTLSurface::MTLSurface(id<MTLDevice> mtlDevice, void* windowData){
		surfaceProxy = [[MTLSurfaceProxy alloc] initWithDevice:mtlDevice
													windowData:windowData];
	}
	
	MTLSurface::~MTLSurface(){
		[surfaceProxy release];
	}
	
	void MTLSurface::setVSync(bool vsync){
		
	}
	
	bool MTLSurface::isVsync() const{
		return false;
	}

	void MTLSurface::resizeBuffers(const mth::vec2ui& size){
		
	}

	void MTLSurface::present(){
		
	}
	
	MTKView* MTLSurface::getView(){
		return surfaceProxy->view;
	}
}
