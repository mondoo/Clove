#include "Clove/Graphics/Metal/MetalRenderPass.hpp"

namespace clove {
    MetalRenderPass::MetalRenderPass(Descriptor descriptor, MTLRenderPipelineColorAttachmentDescriptorArray* colourAttachments, MTLPixelFormat depthPixelFormat)
        : descriptor{ std::move(descriptor) }
        , depthPixelFormat{ depthPixelFormat }
        , colourAttachments{ colourAttachments } {
    }
    
    MetalRenderPass::MetalRenderPass(MetalRenderPass &&other) noexcept = default;
    
    MetalRenderPass& MetalRenderPass::operator=(MetalRenderPass &&other) noexcept = default;
    
    MetalRenderPass::~MetalRenderPass() = default;

	GhaRenderPass::Descriptor const &MetalRenderPass::getDescriptor() const {
		return descriptor;
	}
}
