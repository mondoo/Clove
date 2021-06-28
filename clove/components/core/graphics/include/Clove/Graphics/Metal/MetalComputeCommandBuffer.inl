namespace garlic::clove {
	std::vector<std::function<void(id<MTLComputeCommandEncoder>)>> const &MetalComputeCommandBuffer::getCommands() const {
		return commands;
	}
}
