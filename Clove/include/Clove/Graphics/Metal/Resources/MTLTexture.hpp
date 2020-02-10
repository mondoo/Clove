#pragma once

#include "Clove/Graphics/Core/Resources/Texture.hpp"

#import <MetalKit/MetalKit.h>

namespace clv::gfx::mtl {
	class MTLTexture : public Texture{
		//VARIABLES
	private:
		TextureDescriptor descriptor;
		
		int32_t BPP = 0;
		
		id<MTLTexture> mtlTexture;
		id<MTLSamplerState> mtlSampler;
		
		//FUNCTIONS
	public:
		MTLTexture() = delete;
		MTLTexture(id<MTLDevice> mtlDevice, const TextureDescriptor& descriptor, const std::string& pathToTexture);
		MTLTexture(id<MTLDevice> mtlDevice, const TextureDescriptor& descriptor, const void* data, int32_t BPP);
		
		MTLTexture(const MTLTexture& other) = delete;
		MTLTexture(MTLTexture&& other) noexcept;
		
		MTLTexture& operator=(const MTLTexture& other) = delete;
		MTLTexture& operator=(MTLTexture&& other) noexcept;
		
		virtual ~MTLTexture();
		
		id<MTLTexture> getMTLTexture() const;
		id<MTLSamplerState> getMTLSampler() const;
		
		virtual const TextureDescriptor& getDescriptor() const override;
		
	private:
		void createTexture(id<MTLDevice> mtlDevice, const TextureDescriptor& descriptor, const void* data);
		
		MTLTextureType getTextureType(const TextureStyle stye) const;
		MTLPixelFormat getTextureFormat(const TextureUsage usage) const;
		MTLTextureUsage getTextureUsage(const TextureUsage usage) const;
		MTLStorageMode getTextureStorage(const TextureUsage usage) const;
		
		MTLSamplerMinMagFilter getFilter(const TextureFilter filter) const;
	};
}
