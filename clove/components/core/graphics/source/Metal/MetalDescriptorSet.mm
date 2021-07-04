#include "Clove/Graphics/Metal/MetalDescriptorSet.hpp"

#include "Clove/Graphics/GhaDescriptorSetLayout.hpp"
#include "Clove/Graphics/Metal/MetalBuffer.hpp"
#include "Clove/Graphics/Metal/MetalImageView.hpp"
#include "Clove/Graphics/Metal/MetalSampler.hpp"

#include <Clove/Cast.hpp>

namespace garlic::clove {
	MetalDescriptorSet::MetalDescriptorSet(id<MTLArgumentEncoder> vertexEncoder, id<MTLBuffer> vertexEncoderBuffer, id<MTLArgumentEncoder> pixelEncoder, id<MTLBuffer> pixelEncoderBuffer, id<MTLArgumentEncoder> computeEncoder, id<MTLBuffer> computeEncoderBuffer, std::shared_ptr<GhaDescriptorSetLayout> layout)
		: vertexEncoder{ [vertexEncoder retain] }
		, vertexEncoderBuffer{ [vertexEncoderBuffer retain] }
		, pixelEncoder{ [pixelEncoder retain] }
		, pixelEncoderBuffer{ [pixelEncoderBuffer retain] }
		, computeEndoder{ [computeEndoder retain] }
		, computeEncoderBuffer{ [computeEncoderBuffer retain] }
		, layout{ std::move(layout) } {
	}
	
	MetalDescriptorSet::MetalDescriptorSet(MetalDescriptorSet &&other) noexcept = default;
	
	MetalDescriptorSet &MetalDescriptorSet::operator=(MetalDescriptorSet &&other) noexcept = default;
	
	MetalDescriptorSet::~MetalDescriptorSet() {
		[vertexEncoder release];
		[vertexEncoderBuffer release];
		
		[pixelEncoder release];
		[pixelEncoderBuffer release];
		
		[computeEndoder release];
		[computeEncoderBuffer release];
	}
	
	void MetalDescriptorSet::map(GhaBuffer const &buffer, size_t const offset, size_t const range, DescriptorType const descriptorType, uint32_t const bindingSlot) {
		GhaShader::Stage const shaderStage{ getStageFromBindingSlot(bindingSlot) };
		id<MTLBuffer> mtlBuffer{ polyCast<MetalBuffer const>(&buffer)->getBuffer() };
		
		switch (shaderStage) {
			case GhaShader::Stage::Vertex:
				[vertexEncoder setBuffer:mtlBuffer
								  offset:offset
								 atIndex:bindingSlot];
				break;
			case GhaShader::Stage::Pixel:
				[pixelEncoder setBuffer:mtlBuffer
								 offset:offset
								atIndex:bindingSlot];
				break;
			case GhaShader::Stage::Compute:
				[computeEndoder setBuffer:mtlBuffer
								   offset:offset
								  atIndex:bindingSlot];
				break;
			default:
				CLOVE_ASSERT(false, "{0}: Stage not handled", CLOVE_FUNCTION_NAME_PRETTY)
				break;
		}
	}
	
	void MetalDescriptorSet::map(GhaImageView const &imageView, GhaImage::Layout const layout, uint32_t const bindingSlot) {
		[pixelEncoder setTexture:polyCast<MetalImageView const>(&imageView)->getTexture()
						 atIndex:bindingSlot];
	}

	void MetalDescriptorSet::map(GhaSampler const &sampler, uint32_t const bindingSlot) {
		[pixelEncoder setSamplerState:polyCast<MetalSampler const>(&sampler)->getSamplerState()
							  atIndex:bindingSlot];
	}
	
	GhaShader::Stage MetalDescriptorSet::getStageFromBindingSlot(uint32_t const bindingSlot) {
		for(auto const &binding : layout->getDescriptor().bindings) {
			if(binding.binding == bindingSlot) {
				return binding.stage;
			}
		}
		
		CLOVE_ASSERT(false, "{0}: Could not find binding", CLOVE_FUNCTION_NAME_PRETTY);
		return GhaShader::Stage::Vertex;
	}
}
