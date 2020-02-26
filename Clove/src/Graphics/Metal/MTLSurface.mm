#include "Clove/Graphics/Metal/MTLSurface.hpp"

#include "Clove/Platform/Mac/MacWindow.hpp"
#include "Clove/Graphics/Metal/MTLRenderTarget.hpp"

@implementation MTLView

- (instancetype) initWithFrame:(CGRect)frame {
    self = [super initWithFrame:frame];
    if(self) {
        _metalLayer = (CAMetalLayer*) self.layer;
		return self;
    }
    return self;
}

- (instancetype) initWithCoder:(NSCoder *)aDecoder {
    self = [super initWithCoder:aDecoder];
    if(self) {
        _metalLayer = (CAMetalLayer*) self.layer;
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
		//[view setDepthStencilPixelFormat:MTLPixelFormatDepth32Float];
		
		//[view setDevice:mtlDevice];
		
		MTLRenderPassDescriptor* descriptor = [MTLRenderPassDescriptor new];
        descriptor.colorAttachments[0].loadAction = MTLLoadActionClear;
        descriptor.colorAttachments[0].storeAction = MTLStoreActionStore;
        descriptor.colorAttachments[0].clearColor = MTLClearColorMake(0, 1, 1, 1);

		renderTarget = std::make_shared<MTLRenderTarget>(descriptor);
	}
	
	MTLSurface::MTLSurface(MTLSurface&& other) noexcept = default;
	
	MTLSurface& MTLSurface::operator=(MTLSurface&& other) noexcept = default;
	
	MTLSurface::~MTLSurface(){
		[view release];
	}
	
	void MTLSurface::setVSync(bool vsync){
		[view metalLayer].displaySyncEnabled = vsync;
	}
	
	bool MTLSurface::isVsync() const{
		return [view metalLayer].displaySyncEnabled;
	}

	void MTLSurface::resizeBuffers(const mth::vec2ui& size){
		//TODO:
	}

	void MTLSurface::present(){
		[currentDrawable present];
		currentDrawable = [[view metalLayer] nextDrawable];
		
		renderTarget->updateColourTexture(currentDrawable.texture);
	}
	
	std::shared_ptr<RenderTarget> MTLSurface::getRenderTarget() const{
		return renderTarget;
	}
	
	MTLView* MTLSurface::getMTLView() const{
		return view;
	}
}
