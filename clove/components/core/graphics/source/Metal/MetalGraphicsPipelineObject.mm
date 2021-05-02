#include "Clove/Graphics/Metal/MetalGraphicsPipelineObject.hpp"

namespace garlic::clove {
	MetalGraphicsPipelineObject::MetalGraphicsPipelineObject(id<MTLRenderPipelineState> pipeline, id<MTLDepthStencilState> depthStencil)
		: pipeline{ std::move(pipeline) }
		, depthStencil{ std::move(depthStencil) }{
	}
	
	MetalGraphicsPipelineObject::MetalGraphicsPipelineObject(MetalGraphicsPipelineObject &&other) noexcept = default;
	
	MetalGraphicsPipelineObject& MetalGraphicsPipelineObject::operator=(MetalGraphicsPipelineObject &&other) noexcept = default;
	
	MetalGraphicsPipelineObject::~MetalGraphicsPipelineObject() {
		[pipeline release];
		[depthStencil release];
	}
	
	id<MTLRenderPipelineState> MetalGraphicsPipelineObject::getPipeline() const {
		return pipeline;
	}
}
