#pragma once

namespace clv::gfx{
	class Sprite;
}

namespace clv::gfx{
	class Renderer2D{
		//FUNCTIONS
	public:
		static void initialise();
		static void shutDown();
		
		static void beginScene();
		static void endScene();

		static void submitSprite(const std::shared_ptr<Sprite>& sprite);
		static void submitCharacter(const std::shared_ptr<Sprite>& character);

		static const math::Matrix4f& getSpriteProjection();
	};
}