#include "Clove/Platform/Mac/CloveMac.h"
#include "Clove/Graphics/Metal/MTL.hpp"

#include "Clove/Graphics/Metal/MTLRenderDevice.hpp"
#include "Clove/Graphics/Metal/MTLRenderFactory.hpp"

namespace clv::gfx::mtl{
	std::pair<std::unique_ptr<RenderDevice>, std::unique_ptr<RenderFactory>> initialiseMTL(){
		id<MTLDevice> mtlDevice = MTLCreateSystemDefaultDevice();
		
		auto device = std::make_unique<MTLRenderDevice>();
		auto factory = std::make_unique<MTLRenderFactory>(mtlDevice);

		return std::make_pair(std::move(device), std::move(factory));
	}
}
