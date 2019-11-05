#pragma once

/*
has the swap chain / context (the thing you render to for a window)
*/

namespace clv::gfx{
	class RenderTarget;
}

namespace clv::gfx{
	class Surface{
		//FUNCTIONS
	public:
		//TODO ctor

		virtual void makeCurrent() = 0;
		virtual RenderTarget& getTarget() = 0;
	};
}