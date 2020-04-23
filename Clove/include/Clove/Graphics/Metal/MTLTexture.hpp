#pragma once

#include "Clove/Graphics/Texture.hpp"

#import <MetalKit/MetalKit.h>

namespace clv::gfx::mtl {
	class MTLTexture : public Texture{
		//VARIABLES
	private:
		std::shared_ptr<GraphicsFactory> factory;

		TextureDescriptor descriptor;
		
		int32_t BPP = 0;
		
		id<MTLTexture> mtlTexture;
		id<MTLSamplerState> mtlSampler;
		
		//FUNCTIONS
	public:
		MTLTexture() = delete;
		MTLTexture(std::shared_ptr<GraphicsFactory> factory, id<MTLDevice> mtlDevice, const TextureDescriptor& descriptor, const std::string& pathToTexture);
		MTLTexture(std::shared_ptr<GraphicsFactory> factory, id<MTLDevice> mtlDevice, const TextureDescriptor& descriptor, const void* data, int32_t BPP);
		
		MTLTexture(const MTLTexture& other) = delete;
		MTLTexture(MTLTexture&& other) noexcept;
		
		MTLTexture& operator=(const MTLTexture& other) = delete;
		MTLTexture& operator=(MTLTexture&& other) noexcept;
		
		~MTLTexture();
		
		const std::shared_ptr<GraphicsFactory>& getFactory() const override;

		const TextureDescriptor& getDescriptor() const override;

		id<MTLTexture> getMTLTexture() const;
		id<MTLSamplerState> getMTLSampler() const;
		
	private:
		void createTexture(id<MTLDevice> mtlDevice, const TextureDescriptor& descriptor, const void* data);
		
		MTLTextureType getTextureType(const TextureStyle stye) const;
		MTLPixelFormat getTextureFormat(const TextureUsage usage) const;
		MTLTextureUsage getTextureUsage(const TextureUsage usage) const;
		MTLStorageMode getTextureStorage(const TextureUsage usage) const;
		
		MTLSamplerMinMagFilter getFilter(const TextureFilter filter) const;
	};
}
