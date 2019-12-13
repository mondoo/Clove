#include "Clove/Platform/Mac/CloveMac.h"
#include "Clove/Graphics/Metal/Resources/MTLTexture.hpp"

#include <stb_image.h>

namespace clv::gfx::mtl{
	MTLTexture::MTLTexture(id<MTLDevice> mtlDevice, const TextureDescriptor& descriptor, const std::string& pathToTexture)
		: descriptor(descriptor){
		stbi_set_flip_vertically_on_load(1);

		int width = 0;
		int height = 0;
		unsigned char* localBuffer = stbi_load(pathToTexture.c_str(), &width, &height, &BPP, 4); //4 = RGBA

		this->descriptor.dimensions.x = static_cast<uint32>(width);
		this->descriptor.dimensions.y = static_cast<uint32>(height);

		createTexture(mtlDevice, this->descriptor, localBuffer);

		if(localBuffer){
			stbi_image_free(localBuffer);
		}
	}
	
	MTLTexture::MTLTexture(id<MTLDevice> mtlDevice, const TextureDescriptor& descriptor, const void* data, int32 BPP)
		: descriptor(descriptor), BPP(BPP){
		createTexture(mtlDevice, descriptor, data);
	}
	
	MTLTexture::MTLTexture(MTLTexture&& other) noexcept = default;
	
	MTLTexture& MTLTexture::operator=(MTLTexture&& other) noexcept = default;
	
	MTLTexture::~MTLTexture(){
		[mtlTexture release];
	}
	
	const TextureDescriptor& MTLTexture::getDescriptor() const{
		return descriptor;
	}
	
	void MTLTexture::createTexture(id<MTLDevice> mtlDevice, const TextureDescriptor& descriptor, const void* data){
		//If this is being used as a font then the BPP needs to be 1
		if(descriptor.usage == TextureUsage::Font){
			BPP = 1;
		}
		
		MTLTextureDescriptor* mtlDescriptor = [[MTLTextureDescriptor alloc] init];
		mtlDescriptor.textureType 		= getTextureType(descriptor.style);
		mtlDescriptor.pixelFormat 		= getTextureFormat(descriptor.usage);
		mtlDescriptor.width 			= descriptor.dimensions.x;
		mtlDescriptor.height 			= descriptor.dimensions.y;
		mtlDescriptor.depth 			= 1;
		mtlDescriptor.mipmapLevelCount 	= 1;
		mtlDescriptor.arrayLength 		= descriptor.arraySize;
		mtlDescriptor.usage 			= getTextureUsage(descriptor.usage);
		
		mtlTexture = [mtlDevice newTextureWithDescriptor:mtlDescriptor];
		
		const MTLRegion region = MTLRegionMake2D(0, 0, mtlDescriptor.width, mtlDescriptor.height);
		[mtlTexture replaceRegion:region mipmapLevel:0 withBytes:data bytesPerRow:(BPP * mtlDescriptor.width)];
		
		[mtlDescriptor release];
	}
	
	MTLTextureType MTLTexture::getTextureType(const TextureStyle style) const{
		switch(style){
			case TextureStyle::Default:
				return MTLTextureType2D;
			case TextureStyle::Cubemap:
				return MTLTextureTypeCube;
			default:
				CLV_ASSERT(false, "Unhandled texture type in {0}", CLV_FUNCTION_NAME);
				return MTLTextureType2D;
		}
	}
	
	MTLPixelFormat MTLTexture::getTextureFormat(const TextureUsage usage) const{
		switch(usage){
			case TextureUsage::Default:
			case TextureUsage::RenderTarget_Colour:
				return MTLPixelFormatRGBA8Unorm;

			case TextureUsage::RenderTarget_Depth:
				return MTLPixelFormatR32Float;

			case TextureUsage::Font:
				return MTLPixelFormatR8Unorm;

			default:
				CLV_ASSERT(false, "Unkown type in {0}", CLV_FUNCTION_NAME);
				return MTLPixelFormatRGBA8Unorm;
		}
	}
	
	MTLTextureUsage MTLTexture::getTextureUsage(const TextureUsage usage) const{
		switch(usage){
			case TextureUsage::Default:
			case TextureUsage::Font:
				return MTLTextureUsageShaderRead;
			case TextureUsage::RenderTarget_Colour:
			case TextureUsage::RenderTarget_Depth:
				return MTLTextureUsageRenderTarget | MTLTextureUsageShaderRead | MTLTextureUsageShaderWrite;

			default:
				CLV_ASSERT(false, "Unkown type in {0}", CLV_FUNCTION_NAME);
				return MTLTextureUsageShaderRead;
		}
	}
}
