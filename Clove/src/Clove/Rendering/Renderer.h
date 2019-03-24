#pragma once

#include "Clove/Core.h"
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#define GLCall(x)	clv::GLClearError();\
					x;\
					CLV_ASSERT(clv::GLLogCall(), "{0} {1} {2}", #x, __FILE__, __LINE__)

#include "Clove/Rendering/API/VertexArray.h"
#include "Clove/Rendering/API/IndexBuffer.h"
#include "Clove/Rendering/API/Shader.h"

namespace clv{
	void GLClearError();

	bool GLLogCall();

	class CLV_API Renderer{
		//VARIABLES

		//FUNCTIONS
	public:
		void clear() const;

		void draw(const VertexArray& va, const IndexBuffer& ib, const Shader& shader) const;
	};
}
