namespace clove {
	id<MTLBuffer> MetalDescriptorSet::getVertexBuffer() const {
		return vertexEncoder.backingBuffer;
	}
	
	id<MTLBuffer> MetalDescriptorSet::getPixelBuffer() const {
		return pixelEncoder.backingBuffer;
	}
	
	id<MTLBuffer> MetalDescriptorSet::getComputeBuffer() const {
		return computeEncoder.backingBuffer;
	}
}
