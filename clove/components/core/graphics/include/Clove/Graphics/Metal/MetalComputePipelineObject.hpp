#pragma once

#include "Clove/Graphics/GhaComputePipelineObject.hpp"

#include <MetalKit/MetalKit.h>

namespace clove {
	class MetalComputePipelineObject : public GhaComputePipelineObject {
		//VARIABLES
	private:
        Descriptor descriptor{};
        
		id<MTLComputePipelineState> pipelineState{ nullptr };
		
		//FUNCTIONS
	public:
		MetalComputePipelineObject() = delete;
		MetalComputePipelineObject(Descriptor descriptor, id<MTLComputePipelineState> pipelineState);
		
		MetalComputePipelineObject(MetalComputePipelineObject const &other) = delete;
		MetalComputePipelineObject(MetalComputePipelineObject &&other) noexcept;
		
		MetalComputePipelineObject &operator=(MetalComputePipelineObject const &other) = delete;
		MetalComputePipelineObject &operator=(MetalComputePipelineObject &&other) noexcept;
		
		~MetalComputePipelineObject();
        
        Descriptor const &getDescriptor() const override;
		
		inline id<MTLComputePipelineState> getPipelineState() const;
	};
}

#include "Clove/Graphics/Metal/MetalComputePipelineObject.inl"
