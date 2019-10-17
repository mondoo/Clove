#pragma once

#include "Clove/Graphics/Bindables/Texture.hpp"

#include <glad/glad.h>

namespace clv::gfx{
	class GL4Texture : public Texture{
		//VARIABLES
	private:
		uint32 rendererID = 0;

		std::string filePath = "";
		
		TextureDescriptor descriptor = {};

		int32 BPP = 0;

		uint32 bindingPoint = TBP_None;

		//FUNCTIONS
	public:
		GL4Texture() = delete;
		GL4Texture(const GL4Texture& other) = delete;
		GL4Texture(GL4Texture&& other) noexcept;
		GL4Texture& operator=(const GL4Texture& other) = delete;
		GL4Texture& operator=(GL4Texture&& other) noexcept;
		virtual ~GL4Texture();

		GL4Texture(const std::string& filePath, uint32 bindingPoint);
		GL4Texture(void* bufferData, uint32 bindingPoint, const gfx::TextureDescriptor& descriptor);
		GL4Texture(uint32 bindingPoint, const gfx::TextureDescriptor& descriptor);

		virtual void bind() override;

		virtual uint32 getWidth() const override;
		virtual uint32 getHeight() const override;

		virtual TextureBindingPoint getBindingPoint() const override;
		virtual TextureUsage getUsageType() const override;
		virtual TextureStyle getTextureStyle() const override;

		const uint32 getRenderID() const;

	private:
		void createTexture(void* pixels);

		void createDefaultTexture(const GLenum target, const TextureUsage usage, void* pixels);
		void createCubemapTexture(const TextureUsage usage, void* pixels);

		void setTextureParameters(const GLenum target);

		GLenum getTarget(const TextureStyle style, const uint8 arraySize) const;
		GLenum getInternalFormat(const TextureUsage usage) const;
		GLenum getFormat(const TextureUsage usage);
		GLenum getType(const TextureUsage usage);
	};
}
