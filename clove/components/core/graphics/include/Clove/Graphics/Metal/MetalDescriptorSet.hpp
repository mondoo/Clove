#pragma once

#include "Clove/Graphics/GhaDescriptorSet.hpp"

#include "Clove/Graphics/GhaShader.hpp"

#include <unordered_map>
#include <MetalKit/MetalKit.h>

namespace garlic::clove {
	class GhaDescriptorSetLayout;
}

namespace garlic::clove {
	class MetalDescriptorSet : public GhaDescriptorSet {
		//VARIABLES
	private:
		id<MTLArgumentEncoder> vertexEncoder{ nullptr };
		id<MTLArgumentEncoder> pixelEncoder{ nullptr };
		
		id<MTLBuffer> vertexEncoderBuffer{ nullptr }; /**< The buffer backing the vertex encoder. */
		id<MTLBuffer> pixelEncoderBuffer{ nullptr }; /**< The buffer backing the pixel encoder. */
		
		std::shared_ptr<GhaDescriptorSetLayout> layout{ nullptr };
		
		//FUNCTIONS
	public:
		MetalDescriptorSet() = delete;
		MetalDescriptorSet(id<MTLArgumentEncoder> vertexEncoder, id<MTLBuffer> vertexEncoderBuffer, id<MTLArgumentEncoder> pixelEncoder, id<MTLBuffer> pixelEncoderBuffer, std::shared_ptr<GhaDescriptorSetLayout> layout);
		
		MetalDescriptorSet(MetalDescriptorSet const &other) = delete;
		MetalDescriptorSet(MetalDescriptorSet &&other) noexcept;
		
		MetalDescriptorSet &operator=(MetalDescriptorSet const &other) = delete;
		MetalDescriptorSet &operator=(MetalDescriptorSet &&other) noexcept;
		
		~MetalDescriptorSet();
		
		void map(GhaBuffer const &buffer, size_t const offset, size_t const range, DescriptorType const descriptorType, uint32_t const bindingSlot) override;
		
		void map(GhaImageView const &imageView, GhaSampler const &sampler, GhaImage::Layout const layout, uint32_t const bindingSlot) override;
		void map(std::span<std::shared_ptr<GhaImageView>> imageViews, GhaSampler const &sampler, GhaImage::Layout const layout, uint32_t const bindingSlot) override;
		
		inline id<MTLBuffer> getVertexBuffer() const;
		inline id<MTLBuffer> getPixelBuffer() const;
		
	private:
		GhaShader::Stage getStageFromBindingSlot(uint32_t const bindingSlot);
	};
}

#include "MetalDescriptorSet.inl"
