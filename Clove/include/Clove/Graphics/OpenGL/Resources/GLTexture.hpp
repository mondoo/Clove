#pragma once

#include "Clove/Core/Graphics/Resources/Texture.hpp"

#include <glad/glad.h>

namespace clv::gfx::ogl{
	class GLTexture : public Texture{
		//VARIABLES
	private:
		uint32 textureID = 0;
		
		TextureDescriptor descriptor;

		int32 BPP = 0;

		//FUNCTIONS
	public:
		GLTexture() = delete;
		GLTexture(const TextureDescriptor& descriptor, const std::string& pathToTexture);
		GLTexture(const TextureDescriptor& descriptor, const void* data, int32 BPP);
		GLTexture(const GLTexture& other) = delete;
		GLTexture(GLTexture&& other) noexcept;
		GLTexture& operator=(const GLTexture& other) = delete;
		GLTexture& operator=(GLTexture&& other) noexcept;
		virtual ~GLTexture();

		const uint32 getTextureID() const;
		
		virtual const TextureDescriptor& getDescriptor() const override;

	private:
		void createTexture(const TextureDescriptor& descriptor, const void* data);

		void createDefaultTexture(const GLenum target, const TextureUsage usage, const void* pixels);
		void createCubemapTexture(const TextureUsage usage, const void* pixels);

		GLenum getTarget(const TextureStyle style, const uint8 arraySize) const;
		GLenum getInternalFormat(const TextureUsage usage) const;
		GLenum getFormat(const TextureUsage usage);
		GLenum getType(const TextureUsage usage);
	};
}
