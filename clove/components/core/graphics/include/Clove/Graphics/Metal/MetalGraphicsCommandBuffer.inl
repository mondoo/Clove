namespace garlic::clove {
	std::vector<MetalGraphicsCommandBuffer::RenderPass> const &MetalGraphicsCommandBuffer::getEncodedRenderPasses() const {
		return passes;
	}
}
