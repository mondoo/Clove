#pragma once

namespace clv::gfx{
	class RenderAPI;
	class Context;

	class RenderCommand{
		//VARIABLES
	private:
		static std::unique_ptr<RenderAPI> renderAPI;

		//FUNCTIONS
	public:
		static void clear();
		static void drawIndexed(const unsigned int count);

		static void setClearColour(const math::Vector4f& colour);

		static void setIndexBuffer(const IndexBuffer& buffer);
		
		
		
		
		
		
		
		
		
		
		
		//Do we have the bind calls here then?
		/*
		bindVB(const VerteBuffer& buff)
		bindIB(const IndexBuffer& buff)
		...
		*/

		//I still like the idea of the bindables binding themselves though
		//either way I think there'll have to be a call to the render command - just not sure on the best way to do it
		//	-I think it's fine to put it on the bindables (somehow) - the idea being that renderer will just tell it bind when it's issuing the commands

		//And what about initialisation? 
		//	-really though what are the bindables purpose? just to store data related to that specific bindable really?

		//I think this class would make more sense if it was like the render API - not the other way around. And the bindables need to bind through the renderAPI

		//TODO: Like this?
		static void initialiseRenderAPI(const Context& context);
		
		/*
		Notes:

		How would does the API get initialised?
			-We can get the API type from the context
			-could just have it on this class
				-depends how strict I want to be on the class only doing commands

		should this also handle the commands the bindables are going to make? i.e. CreateBuffer
			-This is only a DX thing though, would they just skip straight to the api calls?
				-Wasn't planning on making the api class static though
			-If calling on this, how does it handle abstracting for opengl? (even though it won't use it)

		*/
	};
}
