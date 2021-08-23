#include "Clove/Graphics/Metal/MetalGraphicsPipelineObject.hpp"

namespace clove {
    MetalGraphicsPipelineObject::MetalGraphicsPipelineObject(Descriptor descriptor, id<MTLRenderPipelineState> pipeline, id<MTLDepthStencilState> depthStencil)
        : descriptor{ std::move(descriptor) }
        , pipeline{ pipeline }
        , depthStencil{ depthStencil }{
    }
    
    MetalGraphicsPipelineObject::MetalGraphicsPipelineObject(MetalGraphicsPipelineObject &&other) noexcept = default;
    
    MetalGraphicsPipelineObject& MetalGraphicsPipelineObject::operator=(MetalGraphicsPipelineObject &&other) noexcept = default;
    
    MetalGraphicsPipelineObject::~MetalGraphicsPipelineObject() = default;

    MetalGraphicsPipelineObject::Descriptor const &MetalGraphicsPipelineObject::getDescriptor() const {
		return descriptor;
	}
}
