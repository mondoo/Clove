#include "Clove/Graphics/Metal/MTLSurface.hpp"

#include "Clove/Platform/Mac/MacWindow.hpp"

namespace clv::gfx::mtl{
	MTLSurface::MTLSurface(id<MTLDevice> mtlDevice, void* windowData){
		clv::plt::MacData* data = reinterpret_cast<clv::plt::MacData*>(windowData);
		
		const NSRect rect = NSMakeRect(0, 0, data->size.x, data->size.y);
		view = [[[MTKView alloc] initWithFrame:rect] autorelease];
		
		[view setDevice:mtlDevice];
	}
	
	MTLSurface::~MTLSurface(){
		[view release];
	}
	
	void MTLSurface::setVSync(bool vsync){
		CALayer* layer = [view layer];
		((CAMetalLayer*)layer).displaySyncEnabled = vsync;
	}
	
	bool MTLSurface::isVsync() const{
		CALayer* layer = [view layer];
		return ((CAMetalLayer*)layer).displaySyncEnabled;
	}

	void MTLSurface::resizeBuffers(const mth::vec2ui& size){
		//TODO:
	}

	void MTLSurface::present(){
		//TODO: Might be quite tricky - the command queue needs the drawable from the view
	}
	
	MTKView* MTLSurface::getView() const{
		return view;
	}
}
