#pragma once

#include "Clove/Graphics/Bindable.hpp"

namespace clv::gfx{
	enum TextureBindingPoint{
		TBP_Albedo		= 1u,
		TBP_Specular	= 2u,
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
