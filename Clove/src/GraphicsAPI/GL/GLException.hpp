#pragma once

#define GLCall(x)	clv::gfx::GLClearError();\
					x;\
					CLV_ASSERT(clv::gfx::GLLogCall(), "{0} {1} {2}", #x, __FILE__, __LINE__)

namespace clv{
	namespace gfx{
		void GLClearError();

		bool GLLogCall();
	}
}
