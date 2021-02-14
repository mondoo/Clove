#pragma once

#include "Clove/Graphics/GhaRenderPass.hpp"

#include <MetalKit/MetalKit.h>

namespace garlic::clove {
	class MetalRenderPass : public GhaRenderPass {
		//VARIABLES
	private:
		MTLRenderPipelineColorAttachmentDescriptorArray* colourAttachments{ nullptr };
		MTLPixelFormat depthPixelFormat;
		
		//FUNCTIONS
	public:
		MetalRenderPass() = delete;
		MetalRenderPass(MTLRenderPipelineColorAttachmentDescriptorArray* colourAttachments, MTLPixelFormat depthPixelFormat);
		
		MetalRenderPass(MetalRenderPass const &other) = delete;
		MetalRenderPass(MetalRenderPass &&other) noexcept;
		
		MetalRenderPass& operator=(MetalRenderPass const &other) = delete;
		MetalRenderPass& operator=(MetalRenderPass &&other) noexcept;
		
		~MetalRenderPass();
		
		MTLRenderPipelineColorAttachmentDescriptorArray* getColourAttachments() const;
		MTLPixelFormat getDepthPixelFormat() const;
	};
}
