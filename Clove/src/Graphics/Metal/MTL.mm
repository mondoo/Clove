#include "Clove/Graphics/Metal/MTL.hpp"

#include "Clove/Graphics/Metal/MTLRenderFactory.hpp"

namespace clv::gfx::mtl{
	std::unique_ptr<RenderFactory> initialiseMTL(){
		id<MTLDevice> mtlDevice = MTLCreateSystemDefaultDevice();
		return std::make_unique<MTLRenderFactory>(mtlDevice);
	}
}
