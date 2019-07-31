#pragma once

#include "Clove/Graphics/Bindable.hpp"

#include "Clove/Graphics/GraphicsTypes.hpp"

namespace clv::gfx{
	enum TextureBindingPoint{
		TBP_Diffuse		= 1u,
		TBP_Specular	= 2u,
		TBP_Sprite		= 3u,
		TBP_FrameBuffer	= 4u,
		TBP_None		= 5u
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

		virtual int getWidth() const = 0;
		virtual int getHeight() const = 0;
	};
}
