#include "Clove/Graphics/Metal/MTLSurface.hpp"

#include "Clove/Platform/Mac/MacWindow.hpp"
#include "Clove/Graphics/Metal/MTLRenderTarget.hpp"

namespace clv::gfx::mtl{
	MTLSurface::MTLSurface(id<MTLDevice> mtlDevice, void* windowData){
		clv::plt::MacData* data = reinterpret_cast<clv::plt::MacData*>(windowData);
		
		const NSRect rect = NSMakeRect(0, 0, data->size.x, data->size.y);
		view = [[MTKView alloc] initWithFrame:rect];
		[view setDepthStencilPixelFormat:MTLPixelFormatDepth32Float];
		view.paused = YES;
		
		[view setDevice:mtlDevice];

		renderTarget = std::make_shared<MTLRenderTarget>([view currentRenderPassDescriptor]);
	}
	
	MTLSurface::MTLSurface(MTLSurface&& other) noexcept = default;
	
	MTLSurface& MTLSurface::operator=(MTLSurface&& other) noexcept = default;
	
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
		[[view currentDrawable] present];
	}
	
	std::shared_ptr<RenderTarget> MTLSurface::getRenderTarget() const{
		return renderTarget;
	}
	
	MTKView* MTLSurface::getMTKView() const{
		return view;
	}
}
