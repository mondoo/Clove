#include "Clove/Graphics/OpenGL/Resources/GLTexture.hpp"

#include <stb_image.h>

namespace clv::gfx::ogl{
	GLTexture::GLTexture(const TextureDescriptor& descriptor, const std::string& pathToTexture)
		: descriptor(descriptor){
		int width = 0;
		int height = 0;
		unsigned char* localBuffer = stbi_load(pathToTexture.c_str(), &width, &height, &BPP, 4); //4 = RGBA
		this->descriptor.dimensions.x = width;
		this->descriptor.dimensions.y = height;

		createTexture(this->descriptor, localBuffer);

		if(localBuffer){
			stbi_image_free(localBuffer);
		}
	}

	GLTexture::GLTexture(const TextureDescriptor& descriptor, const void* data, int32_t BPP)
		: descriptor(descriptor)
		, BPP(BPP){
		createTexture(descriptor, data);
	}

	GLTexture::GLTexture(GLTexture&& other) noexcept = default;

	GLTexture& GLTexture::operator=(GLTexture&& other) noexcept = default;

	GLTexture::~GLTexture(){
		glDeleteTextures(1, &textureID);
	}

	const GLuint GLTexture::getTextureID() const{
		return textureID;
	}

	const TextureDescriptor& GLTexture::getDescriptor() const{
		return descriptor;
	}

	void GLTexture::createTexture(const TextureDescriptor& descriptor, const void* data){
		const GLenum target = getTarget(descriptor.style, descriptor.arraySize);
		const TextureUsage usage = descriptor.usage;
		const GLint filter = getFilter(descriptor.filtering);

		glGenTextures(1, &textureID);
		glBindTexture(target, textureID);

		glTexParameteri(target, GL_TEXTURE_MAG_FILTER, filter);
		glTexParameteri(target, GL_TEXTURE_MIN_FILTER, filter);
		glTexParameteri(target, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(target, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(target, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

		switch(descriptor.style){
			case TextureStyle::Default:
				createDefaultTexture(target, usage, data);
				break;

			case TextureStyle::Cubemap:
				createCubemapTexture(usage, data);
				break;

			default:
				CLV_ASSERT(false, "{0}: Unhandled usage type", CLV_FUNCTION_NAME);
				break;
		}

		glBindTexture(target, 0);
	}

	void GLTexture::createDefaultTexture(const GLenum target, const TextureUsage usage, const void* pixels){
		GLsizei width = static_cast<GLsizei>(descriptor.dimensions.x);
		GLsizei height = static_cast<GLsizei>(descriptor.dimensions.y);
		
		const GLenum internalFormat = getInternalFormat(usage);
		const GLenum format = getFormat(usage);
		const GLenum type = getType(usage);

		const uint8_t arraySize = descriptor.arraySize;

		if(arraySize > 1){
			for(uint8_t i = 0; i < arraySize; ++i){
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

	void GLTexture::createCubemapTexture(const TextureUsage usage, const void* pixels){
		GLsizei width = static_cast<GLsizei>(descriptor.dimensions.x);
		GLsizei height = static_cast<GLsizei>(descriptor.dimensions.y);

		const GLenum internalFormat = getInternalFormat(usage);
		const GLenum format = getFormat(usage);
		const GLenum type = getType(usage);

		const uint8_t arraySize = descriptor.arraySize;
		const uint8_t cubeFaces = 6;

		if(arraySize > 1){
			glTexImage3D(GL_TEXTURE_CUBE_MAP_ARRAY, 0, internalFormat, width, height, cubeFaces * arraySize, 0, format, type, nullptr);
		} else{
			for(uint8_t i = 0; i < cubeFaces; ++i){
				glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, internalFormat, width, height, 0, format, type, pixels);
			}
		}
	}

	GLenum GLTexture::getTarget(const TextureStyle style, const uint8_t arraySize) const{
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

	GLenum GLTexture::getInternalFormat(const TextureUsage usage) const{
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
				return GL_RGBA8;
		}
	}

	GLenum GLTexture::getFormat(const TextureUsage usage){
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
				return GL_RGBA;
		}
	}

	GLenum GLTexture::getType(const TextureUsage usage){
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
				return GL_UNSIGNED_BYTE;
		}
	}

	GLint GLTexture::getFilter(const TextureFilter filter){
		switch(filter){
			case TextureFilter::Nearest:
				return GL_NEAREST;

			case TextureFilter::Linear:
				return GL_LINEAR;

			default:
				CLV_ASSERT(false, "Unkown type in {0}", CLV_FUNCTION_NAME);
				return GL_NEAREST;
		}
	}
}