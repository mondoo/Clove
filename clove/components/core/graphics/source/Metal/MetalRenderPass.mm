#include "Clove/Graphics/Metal/MetalRenderPass.hpp"

namespace garlic::clove {
	MetalRenderPass::MetalRenderPass(MTLRenderPipelineColorAttachmentDescriptorArray* colourAttachments, MTLPixelFormat depthPixelFormat)
		: colourAttachments{ std::move(colourAttachments) }
		, depthPixelFormat{ std::move(depthPixelFormat) }{
	}
	
	MetalRenderPass::MetalRenderPass(MetalRenderPass &&other) noexcept = default;
	
	MetalRenderPass& MetalRenderPass::operator=(MetalRenderPass &&other) noexcept = default;
	
	MetalRenderPass::~MetalRenderPass() = default;
	
	MTLRenderPipelineColorAttachmentDescriptorArray* MetalRenderPass::getColourAttachments() const {
		return colourAttachments;
	}
	
	MTLPixelFormat MetalRenderPass::getDepthPixelFormat() const {
		return depthPixelFormat;
	}
}
