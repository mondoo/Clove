#pragma once

#include "Clove/Graphics/Bindables/Texture.hpp"

namespace clv::gfx{
	class GL4CubemapTexture : public Texture{
		//VARIABLES
	private:
		uint32 rendererID = 0;

		int32 width = 0;
		int32 height = 0;

		uint32 bindingPoint = 0;

		//FUNCTIONS
	public:
		GL4CubemapTexture() = delete;
		GL4CubemapTexture(const GL4CubemapTexture& other) = delete;
		GL4CubemapTexture(GL4CubemapTexture&& other) noexcept;
		GL4CubemapTexture& operator=(const GL4CubemapTexture& other) = delete;
		GL4CubemapTexture& operator=(GL4CubemapTexture&& other) noexcept;
		virtual ~GL4CubemapTexture();

		GL4CubemapTexture(int32 width, int32 height, uint32 bindingPoint);

		virtual void bind() override;

		virtual int32 getWidth() const override;
		virtual int32 getHeight() const override;

		virtual TextureBindingPoint getBindingPoint() const override;
		virtual TextureUsage getUsageType() const override;
	};
}