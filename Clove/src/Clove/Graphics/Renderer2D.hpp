#pragma once

namespace clv::gfx{
	class Sprite;
	class Mesh;
	class Material;

	class Renderer2D{
		//VARIABLES
	private:
		static std::shared_ptr<gfx::Mesh> spriteMesh;
		static std::shared_ptr<gfx::Mesh> characterMesh;

		static std::vector<std::shared_ptr<Sprite>> spritesToRender;
		static std::vector<std::shared_ptr<Sprite>> charactersToRender;

		//FUNCTIONS
	public:
		static void initialise();
		
		static void beginScene();
		static void endScene();

		static void submitSprite(const std::shared_ptr<Sprite>& sprite);
		static void submitCharacter(const std::shared_ptr<Sprite>& character);
	};
}