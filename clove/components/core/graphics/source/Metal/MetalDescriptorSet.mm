#include "Clove/Graphics/Metal/MetalDescriptorSet.hpp"

#include "Clove/Graphics/GhaDescriptorSetLayout.hpp"

namespace garlic::clove {
	MetalDescriptorSet::MetalDescriptorSet(std::shared_ptr<GhaDescriptorSetLayout> layout)
		: layout{ std::move(layout) } {
	}
	
	MetalDescriptorSet::MetalDescriptorSet(MetalDescriptorSet &&other) noexcept = default;
	
	MetalDescriptorSet &MetalDescriptorSet::operator=(MetalDescriptorSet &&other) noexcept = default;
	
	MetalDescriptorSet::~MetalDescriptorSet() = default;
	
	void MetalDescriptorSet::map(GhaBuffer const &buffer, size_t const offset, size_t const range, DescriptorType const descriptorType, uint32_t const bindingSlot) {
		//TODO
		//TODO: Track offsets as well - keep in struct
		//But what about size?
	}
	
	void MetalDescriptorSet::map(GhaImageView const &imageView, GhaSampler const &sampler, GhaImage::Layout const layout, uint32_t const bindingSlot) {
		//TODO
	}
	
	void MetalDescriptorSet::map(std::span<std::shared_ptr<GhaImageView>> imageViews, GhaSampler const &sampler, GhaImage::Layout const layout, uint32_t const bindingSlot) {
		//TODO
	}
}
