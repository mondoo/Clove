#pragma once

namespace clv::gfx{
	class RenderAPI;
	class Context;
	class RenderTarget;

	//NOTE: This will be used to create a multithreaded renderer. But for now it just wraps the API calls
	class RenderCommand{
		//VARIABLES
	private:
		static std::unique_ptr<RenderAPI> renderAPI;

		//FUNCTIONS
	public:
		static void clear();
		static void drawIndexed(const uint32 count);

		static void setClearColour(const math::Vector4f& colour);

		static void setDepthBuffer(bool enabled);

		static void setBlendState(bool enabled);

		static void setRenderTarget(RenderTarget& renderTarget);
		static void resetRenderTarget();

		static void setViewPortSize(uint32 width, uint32 height);

		static void initialiseRenderAPI(const Context& context);
	};
}
