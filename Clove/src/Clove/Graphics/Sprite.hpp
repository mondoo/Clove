#pragma once

#include "Clove/Graphics/MaterialInstance.hpp"

namespace clv::gfx{
	class Sprite{
		//VARIABLES
	private:
		MaterialInstance materialInstance;

		//FUNCTIONS
	public:
		Sprite() = delete;
		Sprite(const Sprite& other);
		Sprite& operator=(const Sprite& other);
		Sprite(Sprite&& other) noexcept;
		Sprite& operator=(Sprite&& other) noexcept;
		~Sprite();

		Sprite(MaterialInstance materialInstance);

		MaterialInstance& getMaterialInstance();

		void bind();
	};
}
