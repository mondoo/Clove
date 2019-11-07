#pragma once

#include "Core/Graphics/GraphicsTypes.hpp"

namespace clv::gfx{
	enum TextureBindingPoint{
		TBP_Albedo = 1u,
		TBP_Specular = 2u,
		TBP_Shadow = 3u,
		TBP_None = 4u
	};

	struct TextureDescriptor{
		TextureBindingPoint bindingPoint = TBP_Albedo;
		TextureStyle style = TextureStyle::Default;
		TextureUsage usage = TextureUsage::Default;
		math::Vector<2, uint32, math::qualifier::defaultp> dimensions = { 0, 0 };
		uint8 arraySize = 1;
	};
}

namespace clv::gfx{
	class Texture{
		//FUNCTIONS
	public:
		virtual void map(const std::string& pathToTexture) = 0;
		virtual void map(void* data) = 0;

		virtual uint32 getWidth() const = 0;
		virtual uint32 getHeight() const = 0;

		virtual TextureBindingPoint getBindingPoint() const = 0;
		virtual TextureUsage getUsageType() const = 0;
		virtual TextureStyle getTextureStyle() const = 0;
	};
}
