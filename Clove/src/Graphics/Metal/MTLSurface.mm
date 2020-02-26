#include "Clove/Graphics/Metal/MTLSurface.hpp"

#include "Clove/Platform/Mac/MacWindow.hpp"
#include "Clove/Graphics/Metal/MTLRenderTarget.hpp"

#include <MetalKit/MetalKit.h>

@implementation MTLView

- (instancetype) initWithFrame:(CGRect)frame {
    self = [super initWithFrame:frame];
    if(self) {
		self.wantsLayer = YES;
        _metalLayer = [CAMetalLayer layer];
		return self;
    }
    return self;
}

- (instancetype) initWithCoder:(NSCoder *)aDecoder {
    self = [super initWithCoder:aDecoder];
    if(self) {
		self.wantsLayer = YES;
        _metalLayer = [CAMetalLayer layer];
		return self;
    }
    return self;
}

@end

namespace clv::gfx::mtl{
	MTLSurface::MTLSurface(id<MTLDevice> mtlDevice, void* windowData){
		clv::plt::MacData* data = reinterpret_cast<clv::plt::MacData*>(windowData);
		
		const NSRect rect = NSMakeRect(0, 0, data->size.x, data->size.y);
		view = [[MTLView alloc] initWithFrame:rect];
		//[view setDepthStencilPixelFormat:MTLPixelFormatDepth32Float];
		
		//[view setDevice:mtlDevice];
		
		currentDrawable = [[view metalLayer] nextDrawable];
		
		MTLRenderPassDescriptor* descriptor = [MTLRenderPassDescriptor new];
        descriptor.colorAttachments[0].loadAction = MTLLoadActionClear;
        descriptor.colorAttachments[0].storeAction = MTLStoreActionStore;
        descriptor.colorAttachments[0].clearColor = MTLClearColorMake(0, 1, 1, 1);
		descriptor.colorAttachments[0].texture = currentDrawable.texture;

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
