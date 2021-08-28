namespace clove {
	std::vector<std::function<void(id<MTLBlitCommandEncoder>)>> const &MetalTransferCommandBuffer::getCommands() const {
		return commands;
	}
}
