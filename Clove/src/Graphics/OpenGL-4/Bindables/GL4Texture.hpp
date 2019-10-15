#pragma once

#include "Clove/Graphics/Bindables/Texture.hpp"

namespace clv::gfx{
	class GL4Texture : public Texture{
		//VARIABLES
	private:
		uint32 rendererID = 0;

		std::string filePath = "";
		TextureUsage usage = TextureUsage::Default;

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

		GL4Texture(const std::string& filePath, uint32 bindingPoint, TextureUsage usageType);
		GL4Texture(void* bufferData, int32 width, int32 height, uint32 bindingPoint, TextureUsage usageType);
		GL4Texture(int32 width, int32 height, uint32 bindingPoint, TextureUsage usageType);

		virtual void bind() override;

		virtual int32 getWidth() const override;
		virtual int32 getHeight() const override;

		virtual TextureBindingPoint getBindingPoint() const override;
		virtual TextureUsage getUsageType() const override;

		const uint32 getRenderID() const;

	private:
		void createTexture(TextureUsage usage, void* pixels);
	};
}
