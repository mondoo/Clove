#pragma once

#define GLCall(x)	clv::GLClearError();\
					x;\
					CLV_ASSERT(clv::GLLogCall(), "{0} {1} {2}", #x, __FILE__, __LINE__)

namespace clv{
	class VertexArray;
	class IndexBuffer;
	class Shader;

	void CLV_API GLClearError();

	bool CLV_API GLLogCall();

	class CLV_API Renderer{
		//VARIABLES

		//FUNCTIONS
	public:
		void clear() const;

		void draw(const VertexArray& va, const IndexBuffer& ib, const Shader& shader) const;
	};
}
