#pragma once

#define GLCall(x)	clv::GLClearError();\
					x;\
					CLV_ASSERT(clv::GLLogCall(), "{0} {1} {2}", #x, __FILE__, __LINE__)

namespace clv{
	void GLClearError();

	bool GLLogCall();
}