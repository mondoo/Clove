#include "clvpch.hpp"
#include "GL4Texture.hpp"

#include <glad/glad.h>
#include <stb_image.h>

namespace clv::gfx{
	GL4Texture::GL4Texture(GL4Texture&& other) noexcept = default;

	GL4Texture& GL4Texture::operator=(GL4Texture&& other) noexcept = default;

	GL4Texture::~GL4Texture(){
		glDeleteTextures(1, &rendererID);
	}

	GL4Texture::GL4Texture(const std::string& filePath, unsigned int bindingPoint)
		: filePath(filePath)
		, bindingPoint(bindingPoint){
		stbi_set_flip_vertically_on_load(1); //Opengl expects our texture to start on the bottom left
		unsigned char* localBuffer = stbi_load(filePath.c_str(), &width, &height, &BPP, 4); //4 = RGBA

		glGenTextures(1, &rendererID);
		glBindTexture(GL_TEXTURE_2D, rendererID);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

		createTexture(TextureUsage::Default, localBuffer);

		glBindTexture(GL_TEXTURE_2D, 0);

		if(localBuffer){
			stbi_image_free(localBuffer);
		}
	}

	GL4Texture::GL4Texture(int width, int height, TextureUsage usageType, unsigned int bindingPoint)
		: width(width)
		, height(height)
		, usageType(usageType)
		, bindingPoint(bindingPoint){
		glGenTextures(1, &rendererID);
		glBindTexture(GL_TEXTURE_2D, rendererID);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		createTexture(usageType, nullptr);

		glBindTexture(GL_TEXTURE_2D, 0);
	}

	void GL4Texture::bind(){
		glActiveTexture(GL_TEXTURE0 + bindingPoint);
		glBindTexture(GL_TEXTURE_2D, rendererID);
	}

	int GL4Texture::getWidth() const{
		return width;
	}

	int GL4Texture::getHeight() const{
		return height;
	}

	TextureUsage GL4Texture::getUsageType() const{
		return usageType;
	}

	const unsigned int GL4Texture::getRenderID() const{
		return rendererID;
	}

	void GL4Texture::createTexture(TextureUsage usage, void* pixels){
		switch(usage){
			case TextureUsage::Default:
				glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, pixels);
				break;

			case TextureUsage::Colour:
				glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, pixels);
				break;

			case TextureUsage::Depth:
				glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, width, height, 0, GL_DEPTH_COMPONENT, GL_UNSIGNED_INT, pixels);
				break;

			case TextureUsage::Stencil:
				glTexImage2D(GL_TEXTURE_2D, 0, GL_STENCIL_INDEX, width, height, 0, GL_STENCIL_INDEX, GL_UNSIGNED_INT, pixels);
				break;

			case TextureUsage::Depth_Stencil:
				glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH24_STENCIL8, width, height, 0, GL_DEPTH_STENCIL, GL_UNSIGNED_INT_24_8, pixels);
				break;

			default:
				CLV_ASSERT(false, "{0}: Unhandled texture type", __func__);
				break;
		}
	}
}