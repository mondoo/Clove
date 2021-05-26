#include "Clove/Graphics/Metal/MetalDescriptorSet.hpp"

#include "Clove/Graphics/GhaDescriptorSetLayout.hpp"
#include "Clove/Graphics/Metal/MetalBuffer.hpp"
#include "Clove/Graphics/Metal/MetalImageView.hpp"
#include "Clove/Graphics/Metal/MetalSampler.hpp"

#include <Clove/Cast.hpp>

namespace garlic::clove {
	MetalDescriptorSet::MetalDescriptorSet(std::shared_ptr<GhaDescriptorSetLayout> layout)
		: layout{ std::move(layout) } {
	}
	
	MetalDescriptorSet::MetalDescriptorSet(MetalDescriptorSet &&other) noexcept = default;
	
	MetalDescriptorSet &MetalDescriptorSet::operator=(MetalDescriptorSet &&other) noexcept = default;
	
	MetalDescriptorSet::~MetalDescriptorSet() = default;
	
	void MetalDescriptorSet::map(GhaBuffer const &buffer, size_t const offset, size_t const range, DescriptorType const descriptorType, uint32_t const bindingSlot) {
		mappedBuffers[bindingSlot] = BufferMapping{
			.buffer = polyCast<MetalBuffer const>(&buffer)->getBuffer(),
			.offset = offset,
		};
	}
	
	void MetalDescriptorSet::map(GhaImageView const &imageView, GhaSampler const &sampler, GhaImage::Layout const layout, uint32_t const bindingSlot) {
		mappedTextures[bindingSlot] = polyCast<MetalImageView const>(&imageView)->getTexture();
		mappedSamplers[bindingSlot] = polyCast<MetalSampler const>(&sampler)->getSamplerState();
	}
	
	void MetalDescriptorSet::map(std::span<std::shared_ptr<GhaImageView>> imageViews, GhaSampler const &sampler, GhaImage::Layout const layout, uint32_t const bindingSlot) {
		std::vector<id<MTLTexture>> mtlTextures{};
		for(auto const &imageView : imageViews) {
			mtlTextures.emplace_back(polyCast<MetalImageView const>(imageView.get())->getTexture());
		}
		
		mappedTextureArrays[bindingSlot] = mtlTextures;
		mappedSamplers[bindingSlot] = polyCast<MetalSampler const>(&sampler)->getSamplerState();
	}
}
