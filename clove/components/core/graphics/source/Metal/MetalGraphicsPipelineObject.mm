#include "Clove/Graphics/Metal/MetalGraphicsPipelineObject.hpp"

namespace garlic::clove {
	MetalGraphicsPipelineObject::MetalGraphicsPipelineObject(Descriptor descriptor, id<MTLRenderPipelineState> pipeline, id<MTLDepthStencilState> depthStencil)
		: descriptor{ std::move(descriptor) }
		, pipeline{ std::move(pipeline) }
		, depthStencil{ std::move(depthStencil) }{
	}
	
	MetalGraphicsPipelineObject::MetalGraphicsPipelineObject(MetalGraphicsPipelineObject &&other) noexcept = default;
	
	MetalGraphicsPipelineObject& MetalGraphicsPipelineObject::operator=(MetalGraphicsPipelineObject &&other) noexcept = default;
	
	MetalGraphicsPipelineObject::~MetalGraphicsPipelineObject() {
		[pipeline release];
		[depthStencil release];
	}

    MetalGraphicsPipelineObject::Descriptor const &MetalGraphicsPipelineObject::getDescriptor() const {
		return descriptor;
	}

	id<MTLRenderPipelineState> MetalGraphicsPipelineObject::getPipeline() const {
		return pipeline;
	}
}
