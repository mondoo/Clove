#pragma once

namespace clv::gfx{
	enum class API{
		OpenGL4,
	#if CLV_PLATFORM_WINDOWS
		DirectX11
	#endif
	};
		
	class Context;
	class IndexBuffer;

	class RenderAPI{
		//FUNCTIONS
	public:
		virtual void clear() = 0;
		virtual void drawIndexed(const unsigned int count) = 0;

		virtual void setClearColour(const math::Vector4f& colour) = 0;

		virtual void setIndexBuffer(const IndexBuffer& buffer) = 0;



		/*virtual void setIndexBuffer*/



		/*
		-How would I do it without the command?
			-probably just grab this guy > cast and away
		
		*/
	};
}