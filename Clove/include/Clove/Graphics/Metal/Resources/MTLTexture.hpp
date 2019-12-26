#pragma once

#include "Clove/Core/Graphics/Resources/Texture.hpp"

namespace clv::gfx::mtl {
	class MTLTexture : public Texture{
		//VARIABLES
	private:
		TextureDescriptor descriptor;
		
		int32 BPP = 0;
		
		id<MTLTexture> mtlTexture;
		
		//FUNCTIONS
	public:
		MTLTexture() = delete;
		MTLTexture(id<MTLDevice> mtlDevice, const TextureDescriptor& descriptor, const std::string& pathToTexture);
		MTLTexture(id<MTLDevice> mtlDevice, const TextureDescriptor& descriptor, const void* data, int32 BPP);
		
		MTLTexture(const MTLTexture& other) = delete;
		MTLTexture(MTLTexture&& other) noexcept;
		
		MTLTexture& operator=(const MTLTexture& other) = delete;
		MTLTexture& operator=(MTLTexture&& other) noexcept;
		
		virtual ~MTLTexture();
		
		id<MTLTexture> getMTLTexture() const;
		
		virtual const TextureDescriptor& getDescriptor() const override;
		
	private:
		void createTexture(id<MTLDevice> mtlDevice, const TextureDescriptor& descriptor, const void* data);
		
		MTLTextureType getTextureType(const TextureStyle stye) const;
		MTLPixelFormat getTextureFormat(const TextureUsage usage) const;
		MTLTextureUsage getTextureUsage(const TextureUsage usage) const;
	};
}
