#include "Clove/Graphics/Metal/MetalRenderPass.hpp"

namespace garlic::clove {
	MetalRenderPass::MetalRenderPass(Descriptor descriptor, MTLRenderPipelineColorAttachmentDescriptorArray* colourAttachments, MTLPixelFormat depthPixelFormat)
		: descriptor{ std::move(descriptor) }{
		, colourAttachments{ std::move(colourAttachments) }
		, depthPixelFormat{ std::move(depthPixelFormat) }{
	}
	
	MetalRenderPass::MetalRenderPass(MetalRenderPass &&other) noexcept = default;
	
	MetalRenderPass& MetalRenderPass::operator=(MetalRenderPass &&other) noexcept = default;
	
	MetalRenderPass::~MetalRenderPass() {
		[colourAttachments release];
	}

	GhaRenderPass::Descriptor const &MetalRenderPass::getDescriptor() const {
		return descriptor;
	}
}
