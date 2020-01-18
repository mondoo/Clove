#pragma once

#include "Tunic/Rendering/MaterialInstance.hpp"

namespace clv::gfx{
	class Texture;
}

namespace tnc::rnd{
	class Sprite{
		//VARIABLES
	private:
		MaterialInstance materialInstance;

		//FUNCTIONS
	public:
		Sprite();
		Sprite(const std::string& pathToTexture);
		Sprite(const std::shared_ptr<clv::gfx::Texture>& texture);
		Sprite(const std::shared_ptr<Material>& material);

		Sprite(const Sprite& other);
		Sprite(Sprite&& other) noexcept;

		Sprite& operator=(const Sprite& other);
		Sprite& operator=(Sprite&& other) noexcept;

		~Sprite();

		MaterialInstance& getMaterialInstance();
	};
}
