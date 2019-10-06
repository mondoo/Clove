#pragma once

#include "Clove/Graphics/Bindables/Texture.hpp"

namespace clv::gfx{
	class DX11CubemapTexture : public Texture{
		//TODO
	public:
		//temp
		DX11CubemapTexture(int32 width, int32 height, TextureUsage usageType, uint32 bindingPoint){}

		virtual int32 getWidth() const override{ return 0; }
		virtual int32 getHeight() const override{ return 0; }

		virtual TextureBindingPoint getBindingPoint() const override{ return TextureBindingPoint::TBP_None; }
		virtual TextureUsage getUsageType() const override{ return TextureUsage::Default; }

	};
}