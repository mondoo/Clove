#pragma once

#include "Clove/Graphics/Core/Resources/Texture.hpp"

#include <glad/glad.h>

namespace clv::gfx::ogl{
	class GLTexture : public Texture{
		//VARIABLES
	private:
		GLuint textureID = 0;
		
		TextureDescriptor descriptor;

		int32_t BPP = 0;

		//FUNCTIONS
	public:
		GLTexture() = delete;
		GLTexture(const TextureDescriptor& descriptor, const std::string& pathToTexture);
		GLTexture(const TextureDescriptor& descriptor, const void* data, int32_t BPP);

		GLTexture(const GLTexture& other) = delete;
		GLTexture(GLTexture&& other) noexcept;

		GLTexture& operator=(const GLTexture& other) = delete;
		GLTexture& operator=(GLTexture&& other) noexcept;

		virtual ~GLTexture();

		const GLuint getTextureID() const;
		
		virtual const TextureDescriptor& getDescriptor() const override;

	private:
		void createTexture(const TextureDescriptor& descriptor, const void* data);

		void createDefaultTexture(const GLenum target, const TextureUsage usage, const void* pixels);
		void createCubemapTexture(const TextureUsage usage, const void* pixels);

		GLenum getTarget(const TextureStyle style, const uint8_t arraySize) const;
		GLenum getInternalFormat(const TextureUsage usage) const;
		GLenum getFormat(const TextureUsage usage);
		GLenum getType(const TextureUsage usage);
		GLint getFilter(const TextureFilter filter);
	};
}
