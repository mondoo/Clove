#pragma once

#include "Core/Graphics/GraphicsTypes.hpp"

namespace clv::gfx{
	struct TextureDescriptor{
		TextureStyle style = TextureStyle::Default;
		TextureUsage usage = TextureUsage::Default;
		math::Vector<2, uint32, math::qualifier::defaultp> dimensions = { 0, 0 };
		uint8 arraySize = 1;
	};

	enum TextureBindingPoint{
		TBP_Albedo = 1u,
		TBP_Specular = 2u,
		TBP_Shadow = 3u,
		TBP_None = 4u
	};
}

namespace clv::gfx{
	class Texture{
		//FUNCTIONS
	public:
		virtual uint32 getWidth() const = 0;
		virtual uint32 getHeight() const = 0;

		virtual TextureBindingPoint getBindingPoint() const = 0;
		virtual TextureUsage getUsageType() const = 0;
		virtual TextureStyle getTextureStyle() const = 0;
	};
}
