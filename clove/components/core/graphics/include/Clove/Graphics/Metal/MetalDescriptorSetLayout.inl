namespace garlic::clove {
	NSArray<MTLArgumentDescriptor *> *MetalDescriptorSetLayout::getVertexDescriptors() const {
		return vertexDescriptors;
	}
	
	NSArray<MTLArgumentDescriptor *> *MetalDescriptorSetLayout::getPixelDescriptors() const {
		return pixelDescriptors;
	}
}
