#pragma once

#include "Core/Graphics/GraphicsTypes.hpp"

namespace clv::gfx{
	struct TextureDescriptor{
		//TODO
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
		Texture() = default;
		Texture(const Texture& other) = delete;
		Texture(Texture&& other) noexcept = default;
		Texture& operator=(const Texture& other) = delete;
		Texture& operator=(Texture&& other) noexcept = default;
		virtual ~Texture() = default;

		virtual uint32 getWidth() const = 0;
		virtual uint32 getHeight() const = 0;

		virtual TextureBindingPoint getBindingPoint() const = 0;
		virtual TextureUsage getUsageType() const = 0;
		virtual TextureStyle getTextureStyle() const = 0;
	};
}
