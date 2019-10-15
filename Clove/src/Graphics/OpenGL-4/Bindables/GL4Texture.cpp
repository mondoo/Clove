#include "GL4Texture.hpp"

#include <glad/glad.h>
#include <stb_image.h>

namespace clv::gfx{
	GL4Texture::GL4Texture(GL4Texture&& other) noexcept = default;

	GL4Texture& GL4Texture::operator=(GL4Texture&& other) noexcept = default;

	GL4Texture::~GL4Texture(){
		glDeleteTextures(1, &rendererID);
	}

	GL4Texture::GL4Texture(const std::string& filePath, uint32 bindingPoint, TextureStyle style)
		: filePath(filePath)
		, bindingPoint(bindingPoint)
		, style(style){
		stbi_set_flip_vertically_on_load(1); //Opengl expects our texture to start on the bottom left
		unsigned char* localBuffer = stbi_load(filePath.c_str(), &width, &height, &BPP, 4); //4 = RGBA

		glGenTextures(1, &rendererID);

		switch(style){
			case TextureStyle::Default:
				glBindTexture(GL_TEXTURE_2D, rendererID);
				setTextureParameters();
				createDefaultTexture(TextureUsage::Default, localBuffer);
				glBindTexture(GL_TEXTURE_2D, 0);
				break;

			case TextureStyle::Cubemap:
				glBindTexture(GL_TEXTURE_CUBE_MAP, rendererID);
				setTextureParameters();
				createCubemapTexture(TextureUsage::Default, localBuffer);
				glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
				break;

			default:
				CLV_ASSERT(false, "{0}: Unhandled usage type", CLV_FUNCTION_NAME);
				break;
		}

		if(localBuffer){
			stbi_image_free(localBuffer);
		}
	}

	GL4Texture::GL4Texture(void* bufferData, int32 width, int32 height, TextureUsage usageType, uint32 bindingPoint, TextureStyle style)
		: width(width)
		, height(height)
		, usageType(usageType)
		, bindingPoint(bindingPoint)
		, style(style){
		glGenTextures(1, &rendererID);

		switch(style){
			case TextureStyle::Default:
				glBindTexture(GL_TEXTURE_2D, rendererID);
				setTextureParameters();
				createDefaultTexture(usageType, bufferData);
				glBindTexture(GL_TEXTURE_2D, 0);
				break;

			case TextureStyle::Cubemap:
				glBindTexture(GL_TEXTURE_CUBE_MAP, rendererID);
				setTextureParameters();
				createCubemapTexture(usageType, bufferData);
				glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
				break;

			default:
				CLV_ASSERT(false, "{0}: Unhandled usage type", CLV_FUNCTION_NAME);
				break;
		}
	}

	GL4Texture::GL4Texture(int32 width, int32 height, TextureUsage usageType, uint32 bindingPoint, TextureStyle style)
		: width(width)
		, height(height)
		, usageType(usageType)
		, bindingPoint(bindingPoint)
		, style(style){
		glGenTextures(1, &rendererID);

		switch(style){
			case TextureStyle::Default:
				glBindTexture(GL_TEXTURE_2D, rendererID);
				setTextureParameters();
				createDefaultTexture(usageType, nullptr);
				glBindTexture(GL_TEXTURE_2D, 0);
				break;

			case TextureStyle::Cubemap:
				glBindTexture(GL_TEXTURE_CUBE_MAP, rendererID);
				setTextureParameters();
				createCubemapTexture(usageType, nullptr);
				glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
				break;

			default:
				CLV_ASSERT(false, "{0}: Unhandled usage type", CLV_FUNCTION_NAME);
				break;
		}
	}

	void GL4Texture::bind(){
		glBindTextureUnit(bindingPoint, rendererID);
	}

	int32 GL4Texture::getWidth() const{
		return width;
	}

	int32 GL4Texture::getHeight() const{
		return height;
	}

	TextureBindingPoint GL4Texture::getBindingPoint() const{
		return static_cast<TextureBindingPoint>(bindingPoint);
	}

	TextureUsage GL4Texture::getUsageType() const{
		return usageType;
	}

	TextureStyle GL4Texture::getTextureStyle() const{
		return style;
	}

	const uint32 GL4Texture::getRenderID() const{
		return rendererID;
	}

	void GL4Texture::createDefaultTexture(TextureUsage usage, void* pixels){
		switch(usage){
			case TextureUsage::Default:
				glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, pixels);
				break;

			case TextureUsage::RenderTarget_Colour:
				glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, pixels);
				break;

			case TextureUsage::RenderTarget_Depth:
				glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, width, height, 0, GL_DEPTH_COMPONENT, GL_FLOAT, pixels);
				break;

			case TextureUsage::Font:
				BPP = 1;
				glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, width, height, 0, GL_RED, GL_UNSIGNED_BYTE, pixels);
				glPixelStorei(GL_UNPACK_ALIGNMENT, BPP); //We have 1 BPP so make sure OpenGL is aware of this
				break;

			default:
				CLV_ASSERT(false, "{0}: Unhandled texture type", CLV_FUNCTION_NAME);
				break;
		}
	}

	void GL4Texture::createCubemapTexture(TextureUsage usage, void* pixels){
		const uint8 cubeFaces = 6;
		switch(usage){
			case TextureUsage::RenderTarget_Depth:
				for(uint8 i = 0; i < cubeFaces; ++i){
					glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_DEPTH_COMPONENT, width, height, 0, GL_DEPTH_COMPONENT, GL_FLOAT, pixels);
				}
				break;

			default:
				CLV_ASSERT(false, "{0}: Unhandled texture type", CLV_FUNCTION_NAME);
				break;
		}
	}

	void GL4Texture::setTextureParameters(){
		switch(style){
			case TextureStyle::Default:
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
				break;
			case TextureStyle::Cubemap:
				glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
				glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
				glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
				glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
				glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
				break;
			default:
				CLV_ASSERT(false, "{0}: Unhandled usage type", CLV_FUNCTION_NAME);
				break;
		}
	}
}