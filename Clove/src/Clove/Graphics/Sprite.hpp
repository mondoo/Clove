#pragma once

namespace clv::gfx{
	/*
	This would just be a texture?
	do we need a sprite class?
	-maybe just for abstraction and expandability

	how wil lthis help refactor the renderer?
	all the renderer needs is the texture, the model and proj data can be handled in the render system
	should this have the sbo for the model / proj?

	
	*/

	class Texture;

	class Sprite{
		//VARIABLES
	private:
		std::shared_ptr<Texture> texture;

		//FUNCTIONS
	public:
		//TODO ctors

		void setTexture(const std::string& path);
		//void setTexture(const std::shared_ptr<Texture>& texture);
	};
}
