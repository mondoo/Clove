#pragma once

#include <queue>

namespace clv::gfx{
	class Sprite;
	class Mesh;
	class Material;

	class Renderer2D{
		//VARIABLES
	private:
		static std::shared_ptr<gfx::Mesh> renderMesh;

		static std::queue<std::shared_ptr<Sprite>> renderQueue;

		/*
		have the above be a piar that has a mesh and a sprite?

		for generic sprites they can use the default mesh
		for specific sprites (fonts) they can use their custom mesh

		would it be a better api to just submit the data and not an entire mesh? (submit quad?)
			- not sure how submit quad will work though, is there another waay the texture data can be given?
			- through a shader value perhaps?
				- yeah, could have a font mesh?

		
		*/

		//FUNCTIONS
	public:
		static void initialise();

		static void beginScene();
		static void endScene();

		static void submitSprite(const std::shared_ptr<Sprite>& sprite);
	};
}