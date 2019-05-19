#pragma once

#include "Clove/Graphics/Drawable.hpp"

namespace clv::gfx{
	class Renderer;

	class Mesh : public Drawable{
		//VARIABLES
	private:
		std::vector<float> vertices;
		std::vector<unsigned int> indices;

		//FUNCTIONS
	public:
		Mesh(Renderer& renderer);
	};
}