#pragma once

#include "Clove/Graphics/Texture.hpp"

#include <glad/glad.h>

namespace clv::gfx::ogl{
	class GLTexture : public Texture{
		//VARIABLES
	private:
		std::shared_ptr<GraphicsFactory> factory;

		TextureDescriptor descriptor;
		
		GLuint textureID = 0;

		int32_t BPP = 0;

		//FUNCTIONS
	public:
		GLTexture() = delete;
		GLTexture(std::shared_ptr<GraphicsFactory> factory, const TextureDescriptor& descriptor, const std::string& pathToTexture);
		GLTexture(std::shared_ptr<GraphicsFactory> factory, const TextureDescriptor& descriptor, const void* data, int32_t BPP);

		GLTexture(const GLTexture& other) = delete;
		GLTexture(GLTexture&& other) noexcept;

		GLTexture& operator=(const GLTexture& other) = delete;
		GLTexture& operator=(GLTexture&& other) noexcept;

		~GLTexture();

		const std::shared_ptr<GraphicsFactory>& getFactory() const override;

		const TextureDescriptor& getDescriptor() const override;

		GLuint getTextureID() const;

	private:
		void createTexture(const TextureDescriptor& descriptor, const void* data);

		void createDefaultTexture(const GLenum target, const TextureUsage usage, const void* pixels);
		void createCubemapTexture(const TextureUsage usage, const void* pixels);
	};
}
