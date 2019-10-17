#include "GL4Texture.hpp"

#include <stb_image.h>

namespace clv::gfx{
	GL4Texture::GL4Texture(GL4Texture&& other) noexcept = default;

	GL4Texture& GL4Texture::operator=(GL4Texture&& other) noexcept = default;

	GL4Texture::~GL4Texture(){
		glDeleteTextures(1, &rendererID);
	}

	GL4Texture::GL4Texture(const std::string& filePath, uint32 bindingPoint)
		: filePath(filePath), bindingPoint(bindingPoint){
		stbi_set_flip_vertically_on_load(1);

		int width = 0;
		int height = 0;
		unsigned char* localBuffer = stbi_load(filePath.c_str(), &width, &height, &BPP, 4); //4 = RGBA
		descriptor.dimensions.x = width;
		descriptor.dimensions.y = height;

		createTexture(localBuffer);
		
		if(localBuffer){
			stbi_image_free(localBuffer);
		}
	}

	GL4Texture::GL4Texture(void* bufferData, uint32 bindingPoint, const gfx::TextureDescriptor& descriptor)
		: bindingPoint(bindingPoint), descriptor(descriptor){
		createTexture(bufferData);
	}

	GL4Texture::GL4Texture(uint32 bindingPoint, const gfx::TextureDescriptor& descriptor)
		: bindingPoint(bindingPoint), descriptor(descriptor){
		createTexture(nullptr);
	}

	void GL4Texture::bind(){
		glBindTextureUnit(bindingPoint, rendererID);
	}

	uint32 GL4Texture::getWidth() const{
		return descriptor.dimensions.x;
	}

	uint32 GL4Texture::getHeight() const{
		return descriptor.dimensions.y;
	}

	TextureBindingPoint GL4Texture::getBindingPoint() const{
		return static_cast<TextureBindingPoint>(bindingPoint);
	}

	TextureUsage GL4Texture::getUsageType() const{
		return descriptor.usage;
	}

	TextureStyle GL4Texture::getTextureStyle() const{
		return descriptor.style;
	}

	const uint32 GL4Texture::getRenderID() const{
		return rendererID;
	}

	void GL4Texture::createTexture(void* pixels){
		glGenTextures(1, &rendererID);

		const GLenum target = getTarget(descriptor.style, descriptor.arraySize);
		const TextureUsage usage = descriptor.usage;

		switch(descriptor.style){
			case TextureStyle::Default:
				glBindTexture(target, rendererID);
				setTextureParameters(target);
				createDefaultTexture(target, usage, pixels);
				glBindTexture(target, 0);
				break;

			case TextureStyle::Cubemap:
				glBindTexture(target, rendererID);
				setTextureParameters(target);
				createCubemapTexture(usage, pixels);
				glBindTexture(target, 0);
				break;

			default:
				CLV_ASSERT(false, "{0}: Unhandled usage type", CLV_FUNCTION_NAME);
				break;
		}
	}

	void GL4Texture::createDefaultTexture(const GLenum target, const TextureUsage usage, void* pixels){
		GLsizei width = static_cast<GLsizei>(descriptor.dimensions.x);
		GLsizei height = static_cast<GLsizei>(descriptor.dimensions.y);
		
		const GLenum internalFormat = getInternalFormat(usage);
		const GLenum format = getFormat(usage);
		const GLenum type = getType(usage);

		const uint8 arraySize = descriptor.arraySize;

		if(arraySize > 1){
			for(uint8 i = 0; i < arraySize; ++i){
				glTexImage3D(target, 0, internalFormat, width, height, i, 0, format, type, pixels);
			}
		} else{
			glTexImage2D(target, 0, internalFormat, width, height, 0, format, type, pixels);
		}

		if(usage == TextureUsage::Font){
			BPP = 1;
			glPixelStorei(GL_UNPACK_ALIGNMENT, BPP); //We have 1 BPP so make sure OpenGL is aware of this
		}
	}

	void GL4Texture::createCubemapTexture(const TextureUsage usage, void* pixels){
		GLsizei width = static_cast<GLsizei>(descriptor.dimensions.x);
		GLsizei height = static_cast<GLsizei>(descriptor.dimensions.y);

		const GLenum internalFormat = getInternalFormat(usage);
		const GLenum format = getFormat(usage);
		const GLenum type = getType(usage);

		const uint8 arraySize = descriptor.arraySize;
		const uint8 cubeFaces = 6;

		if(arraySize > 1){
			for(uint8 i = 0; i < cubeFaces; ++i){
				for(uint8 j = 0; j < arraySize; ++j){
					glTexImage3D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, internalFormat, width, height, j, 0, format, type, pixels);
				}
			}
		} else{
			for(uint8 i = 0; i < cubeFaces; ++i){
				glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, internalFormat, width, height, 0, format, type, pixels);
			}
		}
	}

	void GL4Texture::setTextureParameters(const GLenum target){
		glTexParameteri(target, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(target, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(target, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(target, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(target, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
	}

	GLenum GL4Texture::getTarget(const TextureStyle style, const uint8 arraySize) const{
		switch(style){
			case TextureStyle::Default:
				return (arraySize > 1) ? GL_TEXTURE_2D_ARRAY : GL_TEXTURE_2D;

			case TextureStyle::Cubemap:
				return (arraySize > 1) ? GL_TEXTURE_CUBE_MAP_ARRAY : GL_TEXTURE_CUBE_MAP;
			
			default:
				CLV_ASSERT(false, "{0}: Unhandled texture type", CLV_FUNCTION_NAME);
				break;
		}
		
		return GLenum();
	}

	GLenum GL4Texture::getInternalFormat(const TextureUsage usage) const{
		switch(usage){
			case TextureUsage::Default:
				return GL_RGBA8;

			case TextureUsage::RenderTarget_Colour:
				return GL_RGB;

			case TextureUsage::RenderTarget_Depth:
				return GL_DEPTH_COMPONENT;

			case TextureUsage::Font:
				return GL_RED;

			default:
				CLV_ASSERT(false, "{0}: Unhandled texture type", CLV_FUNCTION_NAME);
		}
	}

	GLenum GL4Texture::getFormat(const TextureUsage usage){
		switch(usage){
			case TextureUsage::Default:
				return GL_RGBA;

			case TextureUsage::RenderTarget_Colour:
				return GL_RGB;

			case TextureUsage::RenderTarget_Depth:
				return GL_DEPTH_COMPONENT;

			case TextureUsage::Font:
				return GL_RED;

			default:
				CLV_ASSERT(false, "{0}: Unhandled texture type", CLV_FUNCTION_NAME);
		}
	}

	GLenum GL4Texture::getType(const TextureUsage usage){
		switch(usage){
			case TextureUsage::Default:
				return GL_UNSIGNED_BYTE;

			case TextureUsage::RenderTarget_Colour:
				return GL_UNSIGNED_BYTE;

			case TextureUsage::RenderTarget_Depth:
				return GL_FLOAT;

			case TextureUsage::Font:
				return GL_UNSIGNED_BYTE;

			default:
				CLV_ASSERT(false, "{0}: Unhandled texture type", CLV_FUNCTION_NAME);
		}
	}
}