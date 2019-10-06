#pragma once

#include "Clove/Graphics/Bindable.hpp"

#include "Clove/Graphics/GraphicsTypes.hpp"

namespace clv::gfx{
	enum TextureBindingPoint{
		TBP_Albedo		= 1u,
		TBP_Specular	= 2u,
		TBP_None        = 3u
	};

	class Texture : public Bindable{
		//FUNCTIONS
	public:
		Texture();
		Texture(const Texture& other) = delete;
		Texture(Texture&& other) noexcept;
		Texture& operator=(const Texture& other) = delete;
		Texture& operator=(Texture&& other) noexcept;
		virtual ~Texture();

		//TODO: should be unsigned
		virtual int32 getWidth() const = 0;
		virtual int32 getHeight() const = 0;

		virtual TextureBindingPoint getBindingPoint() const = 0;
		virtual TextureUsage getUsageType() const = 0;
		virtual TextureStyle getTextureStyle() const = 0;
	};
}
