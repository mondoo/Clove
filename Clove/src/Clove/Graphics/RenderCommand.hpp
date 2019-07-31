#pragma once

namespace clv::gfx{
	class RenderAPI;
	class Context;

	//NOTE: This will be used to create a multithreaded renderer. But for now it just wraps the API calls
	class RenderCommand{
		//VARIABLES
	private:
		static std::unique_ptr<RenderAPI> renderAPI;

		//FUNCTIONS
	public:
		static void clear();
		static void drawIndexed(const unsigned int count);

		static void setClearColour(const math::Vector4f& colour);
		static void setDepthBuffer(bool enabled);
		static void setBlendState(bool enabled);
		static void resetFrameBuffer();

		static void initialiseRenderAPI(const Context& context);
	};
}
