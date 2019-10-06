#include "clvpch.hpp"
#include "GL4CubemapTexture.hpp"

#include <glad/glad.h>

namespace clv::gfx{
	GL4CubemapTexture::GL4CubemapTexture(GL4CubemapTexture&& other) noexcept = default;

	GL4CubemapTexture& GL4CubemapTexture::operator=(GL4CubemapTexture&& other) noexcept = default;

	GL4CubemapTexture::~GL4CubemapTexture() = default;

	GL4CubemapTexture::GL4CubemapTexture(int32 width, int32 height, uint32 bindingPoint)
		: width(width)
		, height(height)
		, bindingPoint(bindingPoint){
		
		glGenTextures(1, &rendererID);
		glBindTexture(GL_TEXTURE_CUBE_MAP, rendererID);

		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

		const uint8 cubeSides = 6;
		for(uint8 i = 0; i < cubeSides; i++){
			//NOTE: cubemaps are currently only used for depth stencils
			glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_DEPTH_COMPONENT, width, height, 0, GL_DEPTH_COMPONENT, GL_FLOAT, nullptr);
		}

		glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
	}

	void GL4CubemapTexture::bind(){
		glBindTexture(GL_TEXTURE_CUBE_MAP, rendererID);
	}

	int32 GL4CubemapTexture::getWidth() const{
		return width;
	}

	int32 GL4CubemapTexture::getHeight() const{
		return height;
	}

	TextureBindingPoint GL4CubemapTexture::getBindingPoint() const{
		return static_cast<TextureBindingPoint>(bindingPoint);
	}

	TextureUsage GL4CubemapTexture::getUsageType() const{
		return TextureUsage::DepthStencil;
	}
}
