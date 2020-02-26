#include "Clove/Graphics/Metal/MTLSurface.hpp"

#include "Clove/Platform/Mac/MacWindow.hpp"
#include "Clove/Graphics/Metal/MTLRenderTarget.hpp"

@implementation MTLView

- (instancetype) initWithFrame:(CGRect)frame {
    self = [super initWithFrame:frame];
    if(self) {
        _metalLayer = (CAMetalLayer*) self.layer;
		//self.layer.delegate = self;
		return self;
    }
    return self;
}

- (instancetype) initWithCoder:(NSCoder *)aDecoder {
    self = [super initWithCoder:aDecoder];
    if(self) {
        _metalLayer = (CAMetalLayer*) self.layer;
		//self.layer.delegate = self;
		return self;
    }
    return self;
}

- (id<CAMetalDrawable>) getNextDrawable{
	return [_metalLayer nextDrawable];
}

@end

namespace clv::gfx::mtl{
	MTLSurface::MTLSurface(id<MTLDevice> mtlDevice, void* windowData){
		clv::plt::MacData* data = reinterpret_cast<clv::plt::MacData*>(windowData);
		
		const NSRect rect = NSMakeRect(0, 0, data->size.x, data->size.y);
		view = [[MTLView alloc] initWithFrame:rect];
		[view setDepthStencilPixelFormat:MTLPixelFormatDepth32Float];
		//view.paused = YES;
		//view.enableSetNeedsDisplay = NO;
		
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
		[view draw];
	}
	
	std::shared_ptr<RenderTarget> MTLSurface::getRenderTarget() const{
		return renderTarget;
	}
	
	MTLView* MTLSurface::getMTKView() const{
		return view;
	}
}
