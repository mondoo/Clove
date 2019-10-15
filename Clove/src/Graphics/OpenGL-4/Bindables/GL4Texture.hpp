#pragma once

#include "Clove/Graphics/Bindables/Texture.hpp"

namespace clv::gfx{
	class GL4Texture : public Texture{
		//VARIABLES
	private:
		uint32 rendererID = 0;

		std::string filePath = "";
		TextureUsage usageType = TextureUsage::Default;
		TextureStyle style = TextureStyle::Default;

		int32 width = 0;
		int32 height = 0;
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

		GL4Texture(const std::string& filePath, uint32 bindingPoint, TextureStyle style);
		GL4Texture(void* bufferData, int32 width, int32 height, TextureUsage usageType, uint32 bindingPoint, TextureStyle style);
		GL4Texture(int32 width, int32 height, TextureUsage usageType, uint32 bindingPoint, TextureStyle style);

		virtual void bind() override;

		virtual int32 getWidth() const override;
		virtual int32 getHeight() const override;

		virtual TextureBindingPoint getBindingPoint() const override;
		virtual TextureUsage getUsageType() const override;
		virtual TextureStyle getTextureStyle() const override;

		const uint32 getRenderID() const;

	private:
		void createDefaultTexture(TextureUsage usage, void* pixels);
		void createCubemapTexture(TextureUsage usage, void* pixels);

		//TODO: Set up enums for nearest / clamp etc
		void setTextureParameters();
	};
}
