#pragma once

#include "Clove/Graphics/Bindables/Bindable.hpp"

namespace clv::gfx{
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
