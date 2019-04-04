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
	private:

		//FUNCTIONS
	public:
		void clear() const;

		void draw(const VertexArray& va, const IndexBuffer& ib, const Shader& shader) const;
	};
}
