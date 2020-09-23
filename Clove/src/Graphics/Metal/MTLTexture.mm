#include "Clove/Graphics/Metal/MTLTexture.hpp"

#include <stb_image.h>
#include <Root/Definitions.hpp>

namespace clv::gfx::mtl{
	MTLTexture::MTLTexture(std::shared_ptr<GraphicsFactory> factory, id<MTLDevice> mtlDevice, const TextureDescriptor& descriptor, const std::string& pathToTexture)
		: factory(std::move(factory))
		, descriptor(descriptor){
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
	
	MTLTexture::MTLTexture(std::shared_ptr<GraphicsFactory> factory, id<MTLDevice> mtlDevice, const TextureDescriptor& descriptor, const void* data, int32_t BPP)
		: factory(std::move(factory))
		, descriptor(descriptor), BPP(BPP){
		createTexture(mtlDevice, descriptor, data);
	}
	
	MTLTexture::MTLTexture(MTLTexture&& other) noexcept = default;
	
	MTLTexture& MTLTexture::operator=(MTLTexture&& other) noexcept = default;
	
	MTLTexture::~MTLTexture(){
		[mtlTexture release];
	}
	
	const std::shared_ptr<GraphicsFactory>& MTLTexture::getFactory() const{
		return factory;
	}

	const TextureDescriptor& MTLTexture::getDescriptor() const{
		return descriptor;
	}

	id<MTLTexture> MTLTexture::getMTLTexture() const{
		return mtlTexture;
	}
	
	id<MTLSamplerState> MTLTexture::getMTLSampler() const{
		return mtlSampler;
	}
	
	void MTLTexture::createTexture(id<MTLDevice> mtlDevice, const TextureDescriptor& descriptor, const void* data){
		//If this is being used as a font then the BPP needs to be 1
		if(descriptor.usage == TextureUsage::Font){
			BPP = 1;
		}
		
		MTLTextureDescriptor* textureDesc = [[MTLTextureDescriptor alloc] init];
		textureDesc.textureType 		= getTextureType(descriptor.style);
		textureDesc.pixelFormat 		= getTextureFormat(descriptor.usage);
		textureDesc.width 				= descriptor.dimensions.x;
		textureDesc.height 				= descriptor.dimensions.y;
		textureDesc.depth 				= 1;
		textureDesc.mipmapLevelCount 	= 1;
		textureDesc.arrayLength 		= descriptor.arraySize;
		textureDesc.usage 				= getTextureUsage(descriptor.usage);
		textureDesc.storageMode			= getTextureStorage(descriptor.usage);
		
		mtlTexture = [mtlDevice newTextureWithDescriptor:textureDesc];
		
		const MTLRegion region = MTLRegionMake2D(0, 0, textureDesc.width, textureDesc.height);
		
		if(data){
			[mtlTexture replaceRegion:region mipmapLevel:0 withBytes:data bytesPerRow:(BPP * textureDesc.width)];
		}
		
		MTLSamplerDescriptor* samplerDesc = [[MTLSamplerDescriptor alloc] init];
		samplerDesc.minFilter 		= getFilter(descriptor.filtering);
		samplerDesc.magFilter		= getFilter(descriptor.filtering);
		samplerDesc.rAddressMode	= MTLSamplerAddressModeClampToEdge;
		samplerDesc.sAddressMode	= MTLSamplerAddressModeClampToEdge;
		samplerDesc.tAddressMode	= MTLSamplerAddressModeClampToEdge;
		
		mtlSampler = [mtlDevice newSamplerStateWithDescriptor:samplerDesc];
		
		[textureDesc release];
		[samplerDesc release];
	}
	
	MTLTextureType MTLTexture::getTextureType(const TextureStyle style) const{
		switch(style){
			case TextureStyle::Default:
				return MTLTextureType2D;
			case TextureStyle::Cubemap:
				return MTLTextureTypeCube;
			default:
				GARLIC_ASSERT(false, "Unhandled texture type in {0}", GARLIC_FUNCTION_NAME);
				return MTLTextureType2D;
		}
	}
	
	MTLPixelFormat MTLTexture::getTextureFormat(const TextureUsage usage) const{
		switch(usage){
			case TextureUsage::Default:
				return MTLPixelFormatRGBA8Unorm;
				
			case TextureUsage::RenderTarget_Colour:
				return MTLPixelFormatBGRA8Unorm;

			case TextureUsage::RenderTarget_Depth:
				return MTLPixelFormatDepth32Float;

			case TextureUsage::Font:
				return MTLPixelFormatR8Unorm;

			default:
				GARLIC_ASSERT(false, "Unkown type in {0}", GARLIC_FUNCTION_NAME);
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
				GARLIC_ASSERT(false, "Unkown type in {0}", GARLIC_FUNCTION_NAME);
				return MTLTextureUsageShaderRead;
		}
	}
	
	MTLStorageMode MTLTexture::getTextureStorage(const TextureUsage usage) const{
		switch(usage){
			case TextureUsage::Default:
			case TextureUsage::Font:
			case TextureUsage::RenderTarget_Colour:
				return MTLStorageModeManaged;
			case TextureUsage::RenderTarget_Depth:
				return MTLStorageModePrivate;

			default:
				GARLIC_ASSERT(false, "Unkown type in {0}", GARLIC_FUNCTION_NAME);
				return MTLStorageModeShared;
		}
	}
	
	MTLSamplerMinMagFilter MTLTexture::getFilter(const TextureFilter filter) const{
		switch(filter){
			case TextureFilter::Nearest:
				return MTLSamplerMinMagFilterNearest;

			case TextureFilter::Linear:
				return MTLSamplerMinMagFilterLinear;

			default:
				GARLIC_ASSERT(false, "Unkown type in {0}", GARLIC_FUNCTION_NAME);
				return MTLSamplerMinMagFilterNearest;
		}
	}
}
