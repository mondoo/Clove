#pragma once

#include "Clove/Graphics/GhaGraphicsPipelineObject.hpp"

#include <MetalKit/MetalKit.h>

namespace garlic::clove{
	class MetalGraphicsPipelineObject : public GhaGraphicsPipelineObject {
		//VARIABLES
	private:
		id<MTLRenderPipelineState> pipeline;
		
		//FUNCTIONS
	public:
		MetalGraphicsPipelineObject() = delete;
		MetalGraphicsPipelineObject(id<MTLRenderPipelineState> pipeline);
		
		MetalGraphicsPipelineObject(MetalGraphicsPipelineObject const &other) = delete;
		MetalGraphicsPipelineObject(MetalGraphicsPipelineObject &&other) noexcept;
		
		MetalGraphicsPipelineObject& operator=(MetalGraphicsPipelineObject const &other) = delete;
		MetalGraphicsPipelineObject& operator=(MetalGraphicsPipelineObject &&other) noexcept;
		
		~MetalGraphicsPipelineObject();
		
		id<MTLRenderPipelineState> getPipeline() const;
	};
}
