#include "Clove/Graphics/Metal/MetalComputePipelineObject.hpp"

namespace garlic::clove {
	MetalComputePipelineObject::MetalComputePipelineObject(Descriptor descriptor, id<MTLComputePipelineState> pipelineState)
        : descriptor{ std::move(descriptor) }
        , pipelineState{ pipelineState } {
	}
	
	MetalComputePipelineObject::MetalComputePipelineObject(MetalComputePipelineObject &&other) noexcept = default;
	
	MetalComputePipelineObject& MetalComputePipelineObject::operator=(MetalComputePipelineObject &&other) noexcept = default;
	
	MetalComputePipelineObject::~MetalComputePipelineObject() = default;
    
    GhaComputePipelineObject::Descriptor const &MetalComputePipelineObject::getDescriptor() const {
        return descriptor;
    }
}
