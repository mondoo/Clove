namespace clove {
    std::optional<size_t> MetalDescriptorSet::getVertexOffset() const {
        if(vertexEncoder.has_value()) {
            return vertexEncoder->offset;
        } else {
            return {};
        }
    }
    
    std::optional<size_t> MetalDescriptorSet::getPixelOffset() const {
        if(pixelEncoder.has_value()) {
            return pixelEncoder->offset;
        } else {
            return {};
        }
    }
    
    std::optional<size_t> MetalDescriptorSet::getComputeOffset() const {
        if(computeEncoder.has_value()) {
            return computeEncoder->offset;
        } else {
            return {};
        }
    }
    
    id<MTLBuffer> MetalDescriptorSet::getBackingBuffer() const {
        return backingBuffer;
    }
}
