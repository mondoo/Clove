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
