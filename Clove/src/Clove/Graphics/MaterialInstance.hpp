#pragma once

//TODO: Move this into the material file???
namespace clv::gfx{
	class MaterialInstance{
		/*
		How this can work:
		Material will set a bunch of base values
		instances will change them

		-should positions etc. (model data) be handled outside of the materials?
			or should it all specifically be through the materials

		-also, how will an instance know what params there are?
		
		-Is it too early for something like this?

		-Maybe I should try and move the shader to the material first???
		*/

		//VARIABLES
	private:
		//Ref to material

		//FUNCTIONS
	public:
		//bind function that'll bind the material underneath

	};
}