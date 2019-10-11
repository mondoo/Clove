#include "GL4Texture.hpp"

#include <glad/glad.h>
#include <stb_image.h>

namespace clv::gfx{
	GL4Texture::GL4Texture(GL4Texture&& other) noexcept = default;

	GL4Texture& GL4Texture::operator=(GL4Texture&& other) noexcept = default;

	GL4Texture::~GL4Texture(){
		glDeleteTextures(1, &rendererID);
	}

	GL4Texture::GL4Texture(const std::string& filePath, uint32 bindingPoint, TextureUsage usageType)
		: filePath(filePath)
		, bindingPoint(bindingPoint)
		, usage(usageType){
		stbi_set_flip_vertically_on_load(true); //Opengl expects our texture to start on the bottom left
		unsigned char* localBuffer = stbi_load(filePath.c_str(), &width, &height, &BPP, 4); //4 = RGBA

		glGenTextures(1, &rendererID);
		glBindTexture(GL_TEXTURE_2D, rendererID);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

		createTexture(usage, localBuffer);

		glBindTexture(GL_TEXTURE_2D, 0);

		if(localBuffer){
			stbi_image_free(localBuffer);
		}
	}

	GL4Texture::GL4Texture(void* bufferData, int32 width, int32 height, uint32 bindingPoint, TextureUsage usageType)
		: width(width)
		, height(height)
		, bindingPoint(bindingPoint)
		, usage(usageType)
		, BPP(1){ //TEMP: putting this to 1
		glGenTextures(1, &rendererID);

		glBindTexture(GL_TEXTURE_2D, rendererID);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

		createTexture(usage, bufferData);

		glBindTexture(GL_TEXTURE_2D, 0);
	}

	GL4Texture::GL4Texture(int32 width, int32 height, uint32 bindingPoint, TextureUsage usageType)
		: width(width)
		, height(height)
		, bindingPoint(bindingPoint)
		, usage(usageType){
		glGenTextures(1, &rendererID);
		glBindTexture(GL_TEXTURE_2D, rendererID);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		createTexture(usageType, nullptr);

		glBindTexture(GL_TEXTURE_2D, 0);
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
		return usage;
	}

	const uint32 GL4Texture::getRenderID() const{
		return rendererID;
	}

	void GL4Texture::createTexture(TextureUsage usage, void* pixels){
		switch(usage){
			case TextureUsage::Default:
				glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, pixels);
				break;

			case TextureUsage::RenderTarget:
				glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, pixels);
				break;

			case TextureUsage::Font:
				glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, width, height, 0, GL_RED, GL_UNSIGNED_BYTE, pixels);
				glPixelStorei(GL_UNPACK_ALIGNMENT, 1); //We have 1 BPP so make sure OpenGL is aware of this
				break;

			default:
				CLV_ASSERT(false, "{0}: Unhandled texture type", CLV_FUNCTION_NAME);
				break;
		}
	}
}