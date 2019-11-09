#pragma once

#include "Core/Graphics/Resources/Texture.hpp"

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
		GLTexture(const TextureDescriptor& descriptor, const std::string& fileToTexture);
		GLTexture(const TextureDescriptor& descriptor, void* data, int32 BPP);
		GLTexture(const GLTexture& other) = delete;
		GLTexture(GLTexture&& other) noexcept;
		GLTexture& operator=(const GLTexture& other) = delete;
		GLTexture& operator=(GLTexture&& other) noexcept;
		virtual ~GLTexture();

		const uint32 getTextureID() const;
		
		virtual const TextureDescriptor& getDescriptor() const override;

	private:
		void createTexture(const TextureDescriptor& descriptor, void* data);

		void createDefaultTexture(const GLenum target, const TextureUsage usage, void* pixels);
		void createCubemapTexture(const TextureUsage usage, void* pixels);

		GLenum getTarget(const TextureStyle style, const uint8 arraySize) const;
		GLenum getInternalFormat(const TextureUsage usage) const;
		GLenum getFormat(const TextureUsage usage);
		GLenum getType(const TextureUsage usage);
	};
}
