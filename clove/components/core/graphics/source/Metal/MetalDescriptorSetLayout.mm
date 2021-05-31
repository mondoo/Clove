#include "Clove/Graphics/Metal/MetalDescriptorSetLayout.hpp"

namespace garlic::clove {
	MetalDescriptorSetLayout::MetalDescriptorSetLayout(Descriptor descriptor, NSArray<MTLArgumentDescriptor *> *vertexDescriptors, NSArray<MTLArgumentDescriptor *> *pixelDescriptors)
		: descriptor{ std::move(descriptor) }
		, vertexDescriptors{ [vertexDescriptors retain] }
		, pixelDescriptors{ [pixelDescriptors retain] } {
	}
	
	MetalDescriptorSetLayout::MetalDescriptorSetLayout(MetalDescriptorSetLayout &&other) noexcept = default;
	
	MetalDescriptorSetLayout& MetalDescriptorSetLayout::operator=(MetalDescriptorSetLayout &&other) noexcept = default;
	
	MetalDescriptorSetLayout::~MetalDescriptorSetLayout() {
		[vertexDescriptors release];
		[pixelDescriptors release];
	}
	
	GhaDescriptorSetLayout::Descriptor const &MetalDescriptorSetLayout::getDescriptor() const {
		return descriptor;
	}
}
