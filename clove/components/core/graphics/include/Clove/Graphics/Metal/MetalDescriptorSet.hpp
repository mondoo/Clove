#pragma once

#include "Clove/Graphics/GhaDescriptorSet.hpp"

#include "Clove/Graphics/GhaShader.hpp"

#include <unordered_map>
#include <MetalKit/MetalKit.h>

namespace clove {
	class GhaDescriptorSetLayout;
}

namespace clove {
	class MetalDescriptorSet : public GhaDescriptorSet {
        //TYPES
    public:
        struct ArgumentEncoder {
            id<MTLArgumentEncoder> encoder{ nullptr };  /**< Encoder object to write commands into. */
            id<MTLBuffer> backingBuffer{ nullptr };     /**< The buffer backing the encoder object. Where data gets written into. */
        };
        
		//VARIABLES
	private:
        ArgumentEncoder vertexEncoder{};
        ArgumentEncoder pixelEncoder{};
        ArgumentEncoder computeEncoder{};

		GhaDescriptorSetLayout const *layout{ nullptr };
		
		//FUNCTIONS
	public:
		MetalDescriptorSet() = delete;
		MetalDescriptorSet(ArgumentEncoder vertexEncoder, ArgumentEncoder pixelEncoder, ArgumentEncoder computeEncoder, GhaDescriptorSetLayout const *layout);
		
		MetalDescriptorSet(MetalDescriptorSet const &other) = delete;
		MetalDescriptorSet(MetalDescriptorSet &&other) noexcept;
		
		MetalDescriptorSet &operator=(MetalDescriptorSet const &other) = delete;
		MetalDescriptorSet &operator=(MetalDescriptorSet &&other) noexcept;
		
		~MetalDescriptorSet();
		
		void map(GhaBuffer const &buffer, size_t const offset, size_t const range, DescriptorType const descriptorType, uint32_t const bindingSlot) override;
		
		void map(GhaImageView const &imageView, GhaImage::Layout const layout, uint32_t const bindingSlot) override;

        void map(GhaSampler const &sampler, uint32_t const bindingSlot) override;

        inline id<MTLBuffer> getVertexBuffer() const;
		inline id<MTLBuffer> getPixelBuffer() const;
		inline id<MTLBuffer> getComputeBuffer() const;
		
	private:
		GhaShader::Stage getStageFromBindingSlot(uint32_t const bindingSlot);
	};
}

#include "MetalDescriptorSet.inl"
