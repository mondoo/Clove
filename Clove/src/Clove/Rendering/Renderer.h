#pragma once

#define GLCall(x)	clv::GLClearError();\
					x;\
					CLV_ASSERT(clv::GLLogCall(), "{0} {1} {2}", #x, __FILE__, __LINE__)

namespace clv{
	class VertexArray;
	class IndexBuffer;
	class Shader;

	void GLClearError();

	bool GLLogCall();

	class Renderer{
		//VARIABLES

		//FUNCTIONS
	public:
		//CLV_API here is temp - i don't think the client should be exposed to the renderer
		CLV_API void clear() const;

		CLV_API void draw(const VertexArray& va, const IndexBuffer& ib, const Shader& shader) const;
	};
}
