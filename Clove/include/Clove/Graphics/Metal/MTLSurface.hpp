#pragma once

#include "Clove/Core/Graphics/Surface.hpp"

@interface MTLSurfaceProxy : NSObject /*<MTKViewDelegate>*/ //don't think I'll need this unless we can only draw when the view wants

@property(readonly) MTKView* view;

- (instancetype)initWithDevice:(id<MTLDevice>)mtlDevice windowData: (void*)windowData;

@end

namespace clv::gfx::mtl{
	class MTLSurface : public Surface{
		//VARIABLES
	private:
		//MTKView* view = nullptr;
		MTLSurfaceProxy* surfaceProxy = nullptr;
		
		//FUNCTIONS
	public:
		//TODO: Ctors
		MTLSurface(id<MTLDevice> mtlDevice, void* windowData);
		virtual ~MTLSurface();
		
		virtual void setVSync(bool vsync) override;
		virtual bool isVsync() const override;

		virtual void resizeBuffers(const mth::vec2ui& size) override;

		virtual void present() override;
		
		MTKView* getView() const;
	};
}
