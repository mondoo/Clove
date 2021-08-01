#include "Clove/Graphics/Metal/MetalComputePipelineObject.hpp"

namespace garlic::clove {
	MetalComputePipelineObject::MetalComputePipelineObject(id<MTLComputePipelineState> pipelineState)
		: pipelineState{ pipelineState } {
	}
	
	MetalComputePipelineObject::MetalComputePipelineObject(MetalComputePipelineObject &&other) noexcept = default;
	
	MetalComputePipelineObject& MetalComputePipelineObject::operator=(MetalComputePipelineObject &&other) noexcept = default;
	
	MetalComputePipelineObject::~MetalComputePipelineObject() = default;
}
