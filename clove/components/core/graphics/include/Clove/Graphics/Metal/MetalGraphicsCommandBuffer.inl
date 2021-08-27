namespace clove {
	std::vector<MetalGraphicsCommandBuffer::RenderPass> const &MetalGraphicsCommandBuffer::getEncodedRenderPasses() const {
		return passes;
	}
	
	CommandBufferUsage MetalGraphicsCommandBuffer::getCommandBufferUsage() const {
		return currentUsage;
	}
}
