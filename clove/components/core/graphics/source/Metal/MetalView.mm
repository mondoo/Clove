#include "Clove/Graphics/Metal/MetalView.hpp"

@implementation MetalView

+ (Class) layerClass {
	return [CAMetalLayer class];
}

- (instancetype) initWithFrame:(CGRect)frame {
	self = [super initWithFrame:frame];
	if(self) {
		self.wantsLayer = YES;
	}
	return self;
}

- (CALayer *) makeBackingLayer {
	_metalLayer = [CAMetalLayer layer];
	[_metalLayer setFramebufferOnly:YES]; //Notifies the layer that the images are only used in framebuffers
	_metalLayer.allowsNextDrawableTimeout = NO; //Always blocks until a drawable is ready
	
	return _metalLayer;
}

- (void) setDevice:(id<MTLDevice>)device {
	[_metalLayer setDevice:device];
}

@end
