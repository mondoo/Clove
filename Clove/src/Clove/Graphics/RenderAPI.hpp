#pragma once

namespace clv::gfx{
	enum class API{
		OpenGL4,
	#if CLV_PLATFORM_WINDOWS
		DirectX11
	#endif
	};
		
	class Context;

	class RenderAPI{

		//FUNCTIONS
	public:
		virtual void clear() = 0;
		virtual void drawIndexed(const unsigned int count) = 0;

		virtual void setClearColour(const math::Vector4f& colour) = 0;
/*
		virtual void setIndexBuffer*/

		//TODO: Should we do this here?
		static std::unique_ptr<RenderAPI> initialiseRenderAPI(const Context& context);
	};
}