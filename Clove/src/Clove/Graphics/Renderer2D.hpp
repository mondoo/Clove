#pragma once

#include <queue>


//TEMP: just hasing out an idea
namespace clv::gfx{
	class Sprite;
	class Mesh;
}
class Drawable2D{ //I think this could be the way to go: it's more expandable because we could have custom drawing logic for the drawables
	virtual void draw(clv::gfx::Mesh* meshToDrawOn) = 0;//????
};
////////////////////////////////////

namespace clv::gfx{
	class Sprite;
	class Mesh;
	class Material;

	class Renderer2D{
		//VARIABLES
	private:
		static std::shared_ptr<gfx::Mesh> renderMesh;

		static std::queue<std::shared_ptr<Sprite>> renderQueue; 
		//Looks like I might need a drawable interface now
		//Although, how do I want the system to work?
		//Would the drawable 2D get given the mesh to change?

		/*
		have the above be a piar that has a mesh and a sprite?

		for generic sprites they can use the default mesh
		for specific sprites (fonts) they can use their custom mesh

		would it be a better api to just submit the data and not an entire mesh? (submit quad?)
			- not sure how submit quad will work though, is there another waay the texture data can be given?
			- through a shader value perhaps?
				- yeah, could have a font mesh?

		
		*/

		/*
		Think I need to change the architecture of drawing 2D - maybe get rid of the sprites?
		*/

		//FUNCTIONS
	public:
		static void initialise();
		
		static void beginScene();
		static void endScene();

		static void submitSprite(const std::shared_ptr<Sprite>& sprite);
		//static void submitQuad(const math::Vector2f& size, /*what to put here?*/);
		
		//have a submitText which will iterator over the text and submit a quad?

		//Could I not just have a submit quad and then adjust the the position?
		//I guess the problem with this is that the font needed to be on a specific line

		/*
		Do i have a submit font and do some sort of construction off of that


		NOOOOO - what am I saying.. I can edit the values in the buffer, I'll just need to cache it off and reset it
		*/
	};
}