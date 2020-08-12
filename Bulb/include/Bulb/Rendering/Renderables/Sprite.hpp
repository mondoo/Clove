#pragma once

//#include "Bulb/Rendering/MaterialInstance.hpp"

namespace clv::gfx {
	class GraphicsFactory;
	class Texture;
}

namespace blb::rnd {
	class Sprite {
		//VARIABLES
	private:
		//MaterialInstance materialInstance;

		//FUNCTIONS
	public:
		Sprite(std::shared_ptr<clv::gfx::GraphicsFactory> graphicsFactory);
		Sprite(const std::string& pathToTexture, std::shared_ptr<clv::gfx::GraphicsFactory> graphicsFactory);
		Sprite(const std::shared_ptr<clv::gfx::Texture>& texture, std::shared_ptr<clv::gfx::GraphicsFactory> graphicsFactory);
		//Sprite(const std::shared_ptr<Material>& material);

		Sprite(const Sprite& other);
		Sprite(Sprite&& other) noexcept;

		Sprite& operator=(const Sprite& other);
		Sprite& operator=(Sprite&& other) noexcept;

		~Sprite();

		//MaterialInstance& getMaterialInstance();
	};
}
