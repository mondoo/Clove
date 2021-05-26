namespace garlic::clove {
	std::shared_ptr<GhaDescriptorSetLayout> const &MetalDescriptorSet::getLayout() const {
		return layout;
	}
	
	std::unordered_map<uint32_t, MetalDescriptorSet::BufferMapping> const &MetalDescriptorSet::getMappedBuffers() const {
		return mappedBuffers;
	}
	
	std::unordered_map<uint32_t, id<MTLTexture>> const &MetalDescriptorSet::getMappedTextures() const {
		return mappedTextures;
	}
	
	std::unordered_map<uint32_t, std::vector<id<MTLTexture>>> const &MetalDescriptorSet::getMappedTextureArrays() const {
		return mappedTextureArrays;
	}
	
	std::unordered_map<uint32_t, id<MTLSamplerState>> const &MetalDescriptorSet::getMappedSamplers() const {
		return mappedSamplers;
	}
}
