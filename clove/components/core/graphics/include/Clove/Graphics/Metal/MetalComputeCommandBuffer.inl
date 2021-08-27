namespace clove {
	std::vector<std::function<void(id<MTLComputeCommandEncoder>)>> const &MetalComputeCommandBuffer::getCommands() const {
		return commands;
	}
	
	CommandBufferUsage MetalComputeCommandBuffer::getCommandBufferUsage() const {
		return currentUsage;
	}
}
