#pragma once

#include "Clove/Graphics/GhaDescriptorSet.hpp"

#include <unordered_map>
#include <MetalKit/MetalKit.h>

namespace garlic::clove {
	class GhaDescriptorSetLayout;
}

namespace garlic::clove {
	class MetalDescriptorSet : public GhaDescriptorSet {
		//VARIABLES
	private:
		std::shared_ptr<GhaDescriptorSetLayout> layout{ nullptr };
		
		std::unordered_map<uint32_t, id<MTLBuffer>> mappedBuffers{};
		std::unordered_map<uint32_t, id<MTLTexture>> mappedTextures{};
		std::unordered_map<uint32_t, std::vector<id<MTLTexture>>> mappedTextureArrays{};
		
		//FUNCTIONS
	public:
		MetalDescriptorSet() = delete;
		MetalDescriptorSet(std::shared_ptr<GhaDescriptorSetLayout> layout);
		
		MetalDescriptorSet(MetalDescriptorSet const &other) = delete;
		MetalDescriptorSet(MetalDescriptorSet &&other) noexcept;
		
		MetalDescriptorSet &operator=(MetalDescriptorSet const &other) = delete;
		MetalDescriptorSet &operator=(MetalDescriptorSet &&other) noexcept;
		
		~MetalDescriptorSet();
		
		void map(GhaBuffer const &buffer, size_t const offset, size_t const range, DescriptorType const descriptorType, uint32_t const bindingSlot) override;
		
		void map(GhaImageView const &imageView, GhaSampler const &sampler, GhaImage::Layout const layout, uint32_t const bindingSlot) override;
		void map(std::span<std::shared_ptr<GhaImageView>> imageViews, GhaSampler const &sampler, GhaImage::Layout const layout, uint32_t const bindingSlot) override;
		
		inline std::shared_ptr<GhaDescriptorSetLayout> const &getLayout() const;
		
		inline std::unordered_map<uint32_t, id<MTLBuffer>> const &getMappedBuffers() const;
		inline std::unordered_map<uint32_t, id<MTLTexture>> const &getMappedTextures() const;
		inline std::unordered_map<uint32_t, std::vector<id<MTLTexture>>> const &getMappedTextureArrays() const;
	};
}

#include "MetalDescriptorSet.inl"
