#include "Clove/Graphics/Metal/MTL.hpp"

#include "Clove/Graphics/Metal/MTLGraphicsFactory.hpp"

#import <MetalKit/MetalKit.h>

namespace clv::gfx::mtl{
	std::shared_ptr<GraphicsFactory> initialiseMTL(){
		id<MTLDevice> mtlDevice = MTLCreateSystemDefaultDevice();
		return std::make_shared<MTLGraphicsFactory>(mtlDevice);
	}
}
