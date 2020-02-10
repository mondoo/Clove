#include "Clove/Graphics/Metal/MTL.hpp"

#include "Clove/Graphics/Metal/MTLGraphicsFactory.hpp"

#import <MetalKit/MetalKit.h>

namespace clv::gfx::mtl{
	std::unique_ptr<GraphicsFactory> initialiseMTL(){
		id<MTLDevice> mtlDevice = MTLCreateSystemDefaultDevice();
		return std::make_unique<MTLGraphicsFactory>(mtlDevice);
	}
}
