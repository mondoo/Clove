#include "Clove/Graphics/Metal/MetalGraphicsPipelineObject.hpp"

namespace garlic::clove {
    MetalGraphicsPipelineObject::MetalGraphicsPipelineObject(id<MTLRenderPipelineState> pipeline, id<MTLDepthStencilState> depthStencil)
        : pipeline{ pipeline }
        , depthStencil{ depthStencil }{
    }
    
    MetalGraphicsPipelineObject::MetalGraphicsPipelineObject(MetalGraphicsPipelineObject &&other) noexcept = default;
    
    MetalGraphicsPipelineObject& MetalGraphicsPipelineObject::operator=(MetalGraphicsPipelineObject &&other) noexcept = default;
    
    MetalGraphicsPipelineObject::~MetalGraphicsPipelineObject() = default;

    MetalGraphicsPipelineObject::Descriptor const &MetalGraphicsPipelineObject::getDescriptor() const {
		return descriptor;
	}
}
